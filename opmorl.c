/*
 ============================================================================
 Name        : opmorl.c
 Author      : zale
 Version     : 0.2-pre
 Copyright   : 2009 Opmorl Team
 Description : Opmorl - main file.
 ============================================================================
 */

#include <stdio.h>
#include "opmorl.h"

/* ARGNOTUSED */
int clean_exit(int dummy) // Can I know the purpose of an argnotused when you could say int clean_exit() {...} ? Don't fly with the ROFLCOPTER !
{
	free_monsters(m_list);
	free_objects(o_list);
	exit(1);
}

void loop()
{
	char c;

	while(42) {
		display_map();
		fflush(stdout);
		c = getchar();
		switch(c) {
		case 'q':
			clean_exit(1);
			break;
		case 'h': case 'j': case 'k': case 'l':
			move_letter(c);
			break;
		case '>':
			new_level();
			break;
		case 'd':
			drop(); 
			break;
#ifdef DEBUG
		case 'L':
			show_monsters();
			printf("HP = %d\n", rodney.hp);
			break;
		case 'O':
			show_objects();
			break;
		case 'W':
			wish();
			break;
#endif
		case ',':
			getObject();
			break;
		case 'z':
			zap_display(); // zap() wrapper.
			break;
		case 'm':
			man();
			break;
		case 'e':
			equip_display(); //equip() wrapper
			break;
		case 'p':
			drink();
			break;
		case 's':
			display_state();
			break;
		case 'o':
			if (lvl_nb%7 == 0) open_door(); //Visit things in towns.
			break;
		}
		turn++;
		while((c = getchar()) != '\n'); // Flush stdin
	}
}

int main(void)
{
	srand(time(NULL));
	printf("%d", min3(3, 2, 1));
	first_init();
	fill_map();

	check_visit();

	loop();
	
	clean_exit(0);
	 
	return 1;
}
