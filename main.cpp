/*
 * main.cpp
 *
 *  Created on: Nov 22, 2009
 *      Author: Yogesh Arora
 */

#include "ChainedIndex.h"
#include "FRec.h"

int main(int arc, char* argv[])
{
	ChainedIndex index("text.idx", "test.dat");
	FRec rec;
	rec.num=100;
	index.insertRecord(rec);
	rec.num= 1101;
	index.insertRecord(rec);
	index.insertRecord(rec);
}
