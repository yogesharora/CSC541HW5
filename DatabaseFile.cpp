/*
 * DatabaseFile.cpp
 *
 *  Created on: Nov 22, 2009
 *      Author: Yogesh Arora
 */

#include "DatabaseFile.h"
#include "filereader.h"

DatabaseFile::DatabaseFile(const string& f) :
	fileName(f)
{
	// TODO
	filereader fileReader;
	fileReader.open(fileName, 'w');
	fileReader.close();
}

DatabaseFile::~DatabaseFile()
{

}

long DatabaseFile::insert(const FRec& rec)
{
	filereader fileReader;
	fileReader.open(fileName, 'x');
	fileReader.clear();
	fileReader.seek(0, END);
	long offset = fileReader.offset();
	fileReader.write_raw((char*)&rec, sizeof(FRec));
	fileReader.close();
	return offset;
}

void DatabaseFile::load(long offset, FRec &rec)
{
	filereader fileReader;
	fileReader.open(fileName, 'r');
	fileReader.seek(offset, BEGIN);
	fileReader.read_raw((char*)&rec, sizeof(FRec));
	fileReader.close();
}
