/*
 * util.c
 *
 *  Created on: 8 dec. 2009
 *      Author: zale
 *
 *	Copyright : Opmorl, inc.
 */

#include "opmorl.h"

int min(int a, int b)
{
	return a<b ? a:b; /* OMG TERNARY COMMENT - ZALE : ternary rocks ! */
}

int max(int a, int b)
{
	return a>b ? a:b;
}

int min3(int a, int b, int c)
{
	return a<b?a<c?a:c:b<c?b:c;
}

int max3(int a, int b, int c)
{
	return a>b?a>c?a:c:b>c?b:c;
}

int rnd_max(int a, int b)
{
	return rand()%(b-a+1) + a; /* yeah, better */
}

void swap(int * a, int * b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void win()
{
	printf("You hear a hollow voice :\n");
	printf("\"Mortal, you have defeated the infamous wizard of Yendor. As a");
	printf(" proof of my gratitude, I will grant you immortality !\"\n");
	printf("You are now immortal.\n\n");

	printf("You have won.\n");
	printf("Congratulations ! You have defeated the WoY at experience level"
			" %d, with %d gold after %d turns.\n", rodney.exp_lvl, rodney.gold, turn);
	clean_exit(0);
}
