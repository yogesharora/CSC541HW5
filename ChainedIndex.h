/*
 * ChainedIndex.h
 *
 *  Created on: Nov 22, 2009
 *      Author: Yogesh Arora
 */

#ifndef CHAINEDINDEX_H_
#define CHAINEDINDEX_H_

#include "str.h"
#include "FRec.h"
#include "filereader.h"
#include "DatabaseFile.h"
#include "globals.h"

class ChainedIndex
{
	const string& fileName;
	DatabaseFile dbFile;

	struct ChNode // Chained hash table node
	{
		int key; // Key of record node represents
		long recOffset; // Offset of corresponding record in database file
		long nextOffset; // Offset of nextOffset node in this chain (-1 for none)

		ChNode() :
			key(INVALID_KEY), recOffset(INVALID_OFFSET), nextOffset(INVALID_OFFSET)
		{
		}

		ChNode(int k, int r) :
			key(k), recOffset(r), nextOffset(INVALID_OFFSET)
		{

		}

	};

	int hash(int key);
	bool findKey(int key, FRec& result, filereader& file);
public:
	ChainedIndex(const string& fileName, const string& dbFileName);
	~ChainedIndex();

	void insertRecord(const FRec& rec);
	bool findKey(int key, FRec& result);

	void deleteKey(int key);
	void printIndex();
};

#endif /* CHAINEDINDEX_H_ */
