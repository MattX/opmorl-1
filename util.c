/*
 * util.c
 *
 *  Created on: 8 déc. 2009
 *      Author: matthieu
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

int rnd_max(int a, int b)
{
	return rand()%(b-a+1) + a; /* yeah, better */
}
