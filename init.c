/*
 * init.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 */

#include "opmorl.h"

/** first_init - init vars
 *
 */

void first_init()
{
	srand(time(NULL));

	rodney.gold = 0;
	rodney.posx = 1;
	rodney.posy = 1;
	rodney.sword_b = 0;
	rodney.bow_b = 0;

	m_list = NULL;

	going_up = 0;
	lvl_nb = 0;
	turn = 0;

	//Now we init monsters :
	/*m_default =
	{ { ""*/
}
