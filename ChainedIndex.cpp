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

	file.open(fileName, 'x');
	file.seek(0, END);
	if (file.offset() == 0)
	{
		node.key = -1;
		for (int i = 0; i < HASH_TABLE_SIZE; i++)
		{
			file.write_raw((char *) &node, sizeof(ChNode));
		}
	}

}

ChainedIndex::~ChainedIndex()
{
}

int ChainedIndex::hash(int key)
{
	return key % HASH_TABLE_SIZE;
}

void ChainedIndex::insertRecord(const FRec& rec)
{
	filereader file;
	file.open(fileName, 'x');
	int found = false;
	int next = hash(rec.num) * sizeof(ChNode);
	ChNode prevIndex;
	// if it is first record
	file.seek(next, BEGIN);
	file.read_raw((char *) &prevIndex, sizeof(ChNode));

	if (prevIndex.key != INVALID_KEY)
	{
		do
		{
			file.seek(next, BEGIN);
			file.read_raw((char *) &prevIndex, sizeof(ChNode));
			if (prevIndex.key == rec.num)
			{
				printf("Record num is a duplicate.");
				return;
			}
			next = prevIndex.nextOffset;

		} while (next != INVALID_OFFSET);

		// we dont have duplicate
		// write the database record
		int newRecOffset= dbFile.insert(rec);
		int prevIndexOffset = file.offset() - sizeof(ChNode);

		// write new index record
		file.seek(0, END);
		int newIndexOffset = file.offset();
		ChNode newIndex(rec.num, newRecOffset);
		file.write_raw((char*) &newIndex, sizeof(ChNode));

		// write the prev index record
		prevIndex.nextOffset = newIndexOffset;
		file.seek(prevIndexOffset, END);
		file.write_raw((char*) &prevIndex, sizeof(ChNode));
	}
	else
	{
		// its the first record of the chain
		int newRecOffset= dbFile.insert(rec);
		int prevIndexOffset = file.offset() - sizeof(ChNode);
		// write the database record

		// write the prev index record
		prevIndex.key = rec.num;
		prevIndex.recOffset = newRecOffset;
		file.seek(prevIndexOffset, END);
		file.write_raw((char*) &prevIndex, sizeof(ChNode));
	}
	file.close();
}

void ChainedIndex::deleteKey(int key)
{

}

void ChainedIndex::printIndex()
{

}
