/*
 * DataStructures.h
 *
 *  Created on: Nov 22, 2009
 *      Author: Yogesh Arora
 */

#ifndef FREC_H_
#define FREC_H_
#include "str.h"

struct FRec
{
	// Financial record structure
	int num; // Transaction number
	float amt; // Amount of the transaction
	char type; // Transaction type (D=debit, C=credit)
	int acct; // Account used for transaction
	int hour; // Hour of transaction (0-23)
	int min; // Minute of transaction (0-59)
	int day; // Day of transaction (0-364)
	int year; // Year of transaction (0000-9999)

	FRec(int n,	float a, char t, int ac, int h, int m,int d,int y) :
		num(n), amt(a), type(t), acct(ac), hour(h), min(m), day(d), year(y)
	{}

	FRec(): num(0), amt(0), type('D'), acct(0), hour(0), min(0), day(0), year(0)
	{}
};
#endif /* FREC_H_ */
