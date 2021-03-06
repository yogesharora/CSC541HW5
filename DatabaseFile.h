/*
 * DatabaseFile.h
 *
 *  Created on: Nov 22, 2009
 *      Author: Yogesh Arora
 */

#ifndef DATABASEFILE_H_
#define DATABASEFILE_H_

#include "str.h"
#include "FRec.h"

class DatabaseFile
{
	string fileName;
public:
	DatabaseFile(const string& fileName);
	~DatabaseFile();

	long insert(const FRec& rec);
	void load(long offset, FRec& rec);
};

#endif /* DATABASEFILE_H_ */
