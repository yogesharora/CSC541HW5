/*
 * main.cpp
 *
 *  Created on: Nov 22, 2009
 *      Author: Yogesh Arora
 */

#include "ChainedIndex.h"
#include "FRec.h"
#include "globals.h"

int main(int arc, char* argv[])
{
	ChainedIndex index("text.idx", "test.dat");
	filereader consoleReader;
	consoleReader.open('r');
	while (!consoleReader.eof())
	{
		string buff;
		consoleReader.getline(buff, 0);
		string commands[9];
		buff.token(commands, 9, COMMAND_SEPARATOR);
		string command = commands[0];
		if (command == "add")
		{
			FRec rec;
			rec.num = atoi(commands[1]);
			rec.amt = atof(commands[2]);
			rec.type = commands[3][0];
			rec.acct = atoi(commands[4]);
			rec.hour = atoi(commands[5]);
			rec.min = atoi(commands[6]);
			rec.day = atoi(commands[7]);
			rec.year = atoi(commands[8]);
			if (!index.insertRecord(rec))
			{
				printf("Record %d is a duplicate.\n", rec.num);
			}
		}
		else if (command == "find")
		{
			FRec rec;
			int inputKey = atoi(commands[1]);
			if (index.findKey(inputKey, rec))
			{
				rec.print();
			}
			else
			{
				printf("Record %d does not exist.\n", inputKey);
			}
		}
		else if (command == "delete")
		{
			FRec rec;
			int inputKey = atoi(commands[1]);
			if (!index.deleteKey(inputKey))
			{
				printf("Record %d does not exist.\n", inputKey);
			}
		}
		else if (command == "print")
		{

		}
		else if (command == "end")
		{
			break;
		}
	}
	consoleReader.close();
}
