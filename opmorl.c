/*
 ============================================================================
 Name        : opmorl.c
 Author      : zale
 Version     : 0.2-pre
 Copyright   : 2009 Opmorl Team
 Description : Opmorl - temporary main file.
 ============================================================================
 */

#include <stdio.h>
#include "opmorl.h"

/* ARGNOTUSED */
int clean_exit(int dummy)
{
	free_monsters(m_list);
	exit(EXIT_SUCCESS);
}

void loop()
{
	char c;

	while(1) {
		display_map();
		fflush(stdout);
		c = getchar();
		switch(c) {
		case 'q':
			clean_exit(0);
			break;
		case 'h': case 'j': case 'k': case 'l':
			move_letter(c);
			break;
		}
		while((c = getchar()) != '\n'); // Flush stdin
	}
}

int main()
{
	fill_map();
	first_init();

	Monster m = { 1, 1, 0, 0, 0, 0, 0, 0, 20 };
	add_monster(m, -1, -1);
	m.hp = 10;
	add_monster(m, -1, -1);
	printf("%d %d %d - %d\n", m_list->posx, m_list->posy, m_list->hp, m_list->next->hp);
	free_monsters(m_list);

	loop();

	return EXIT_SUCCESS;
}
