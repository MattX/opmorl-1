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
