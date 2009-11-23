/*
 * ChainedIndex.cpp
 *
 *  Created on: Nov 22, 2009
 *      Author: Yogesh Arora
 */

#include "ChainedIndex.h"
#include "filereader.h"
#include "globals.h"

ChainedIndex::ChainedIndex(const string& f, const string& dbFileName) :
	fileName(f), dbFile(dbFileName)
{
	filereader file;
	ChNode node;

	// TODO
	//	int rc = file.open(fileName, 'x');
	//	if(rc==0)
	//	{
	// file does not exist
	file.open(fileName, 'w');
	file.close();
	file.open(fileName, 'x');
	//	}
	file.seek(0, END);
	if (file.offset() == 0)
	{
		for (int i = 0; i < HASH_TABLE_SIZE; i++)
		{
			file.write_raw((char *) &node, sizeof(ChNode));
		}
	}
	file.close();
}

ChainedIndex::~ChainedIndex()
{
}

int ChainedIndex::hash(int key)
{
	return key % HASH_TABLE_SIZE;
}

bool ChainedIndex::insertRecord(const FRec& rec)
{
	bool inserted = true;
	filereader file;
	file.open(fileName, 'x');
	long firstIndexOffset = hash(rec.num) * sizeof(ChNode);
	ChNode firstIndexRecord;
	file.seek(firstIndexOffset, BEGIN);
	file.read_raw((char *) &firstIndexRecord, sizeof(ChNode));

	// if it is first record of the chain
	if (firstIndexRecord.key != INVALID_KEY)
	{
		long curIndexOffset = firstIndexOffset;
		ChNode curIndexRecord = firstIndexRecord;
		do
		{
			file.seek(curIndexOffset, BEGIN);
			file.read_raw((char *) &curIndexRecord, sizeof(ChNode));

			if (curIndexRecord.key == rec.num)
			{
				inserted = false;
				file.close();
				return inserted;
			}

			curIndexOffset = curIndexRecord.nextOffset;
		} while (curIndexOffset != INVALID_OFFSET);

		// we dont have duplicate
		// write the database record
		int newRecOffset = dbFile.insert(rec);
		int prevIndexOffset = file.offset() - sizeof(ChNode);

		// write new index record
		file.seek(0, END);
		int newIndexOffset = file.offset();
		ChNode newIndex(rec.num, newRecOffset);
		file.write_raw((char*) &newIndex, sizeof(ChNode));

		// write the cur index record
		curIndexRecord.nextOffset = newIndexOffset;
		file.seek(prevIndexOffset, BEGIN);
		file.write_raw((char*) &curIndexRecord, sizeof(ChNode));
	}
	else
	{
		// its the first record of the chain
		int newRecOffset = dbFile.insert(rec);
		// write the database record

		// write the prev index record
		firstIndexRecord.key = rec.num;
		firstIndexRecord.recOffset = newRecOffset;
		file.seek(firstIndexOffset, BEGIN);
		file.write_raw((char*) &firstIndexRecord, sizeof(ChNode));
	}
	file.close();
	return inserted;
}

bool ChainedIndex::findKey(int key, FRec& result)
{
	bool found = false;
	filereader file;
	file.open(fileName, 'x');
	long firstIndexOffset = hash(key) * sizeof(ChNode);
	ChNode firstIndexRecord;
	file.seek(firstIndexOffset, BEGIN);
	file.read_raw((char *) &firstIndexRecord, sizeof(ChNode));

	if (firstIndexRecord.key != INVALID_KEY)
	{
		long curIndexOffset = firstIndexOffset;
		ChNode curIndexRecord = firstIndexRecord;
		do
		{
			file.seek(curIndexOffset, BEGIN);
			file.read_raw((char *) &curIndexRecord, sizeof(ChNode));

			if (curIndexRecord.key == key)
			{
				found = true;
				// read record from DBfile
				dbFile.load(curIndexRecord.recOffset, result);
				break;
			}

			curIndexOffset = curIndexRecord.nextOffset;
		} while (curIndexOffset != INVALID_OFFSET);
	}

	file.close();
	return found;
}

bool ChainedIndex::deleteKey(int key)
{
	bool found = false;
	filereader file;
	file.open(fileName, 'x');
	long firstIndexOffset = hash(key) * sizeof(ChNode);
	ChNode firstIndexRecord;
	file.seek(firstIndexOffset, BEGIN);
	file.read_raw((char *) &firstIndexRecord, sizeof(ChNode));

	if (firstIndexRecord.key != INVALID_KEY)
	{
		long curIndexOffset = firstIndexOffset;
		ChNode curIndexRecord = firstIndexRecord;
		long prevIndexOffset;
		do
		{
			file.seek(curIndexOffset, BEGIN);
			file.read_raw((char *) &curIndexRecord, sizeof(ChNode));

			if (curIndexRecord.key == key)
			{
				found = true;
				// if it is first record of chain
				if(curIndexRecord.key==firstIndexRecord.key)
				{
					if(curIndexRecord.nextOffset==INVALID_OFFSET)
					{
						// write an index record there
						ChNode empty;
						long offsetToWite = file.offset() - sizeof(ChNode);
						file.seek(offsetToWite, BEGIN);
						file.write_raw((char *) &empty, sizeof(ChNode));
					}
					else
					{
						// read the next in chain and place it here
						ChNode empty;
						long offsetToWite = file.offset() - sizeof(ChNode);
						file.seek(curIndexRecord.nextOffset, BEGIN);
						file.read_raw((char *) &empty, sizeof(ChNode));
						file.seek(offsetToWite, BEGIN);
						file.write_raw((char *) &empty, sizeof(ChNode));
					}
				}
				else
				{
					ChNode prevIndexRecord;
					file.seek(prevIndexOffset, BEGIN);
					file.read_raw((char *) &prevIndexRecord, sizeof(ChNode));
					prevIndexRecord.nextOffset=curIndexRecord.nextOffset;
					file.seek(prevIndexOffset, BEGIN);
					file.write_raw((char *) &prevIndexRecord, sizeof(ChNode));
				}

				break;
			}

			prevIndexOffset = curIndexOffset;
			curIndexOffset = curIndexRecord.nextOffset;
		} while (curIndexOffset != INVALID_OFFSET);
	}

	file.close();
	return found;
}

void ChainedIndex::printIndex()
{
	filereader file;
	file.open(fileName, 'x');

	for (int key = 0; key < HASH_TABLE_SIZE; key++)
	{
		printf("%d:" ,key);
		long firstIndexOffset = hash(key) * sizeof(ChNode);
		ChNode firstIndexRecord;
		file.seek(firstIndexOffset, BEGIN);
		file.read_raw((char *) &firstIndexRecord, sizeof(ChNode));

		if (firstIndexRecord.key != INVALID_KEY)
		{
			long curIndexOffset = firstIndexOffset;
			ChNode curIndexRecord = firstIndexRecord;
			do
			{
				file.seek(curIndexOffset, BEGIN);
				file.read_raw((char *) &curIndexRecord, sizeof(ChNode));

				printf(" %d/%ld", curIndexRecord.key, curIndexRecord.recOffset);
				curIndexOffset = curIndexRecord.nextOffset;
			} while (curIndexOffset != INVALID_OFFSET);
		}
		printf("\n");
	}
	file.close();
}
