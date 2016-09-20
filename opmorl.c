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
	free_objects(o_list);
	exit(EPIC_WIN);
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
		case '>':
			new_level();
			break;
		case 'L':
			show_monsters();
			printf("HP = %d\n", rodney.hp);
			break;
		case 'O':
			show_objects();
			break;
		case 'd':
			drop(); 
			break;
		case 'W':
			wish();
			break;
		case ',':
			getObject();
			break;
		//TODO: Write equip() wrapper.
		}
		turn++;
		while((c = getchar()) != '\n'); // Flush stdin
	}
}

int main()
{
	srand(time(NULL));

	first_init();
	fill_map();

	check_visit();

	loop();
	
	clean_exit(0);

	return EXIT_SUCCESS;
}
