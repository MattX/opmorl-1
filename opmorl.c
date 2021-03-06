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
						  // WELL, (1) read signal() doc (2) read lint doc.
{
	int i;
	save();
	free_monsters(m_list);
	free_objects(o_list);
	if(weapon) free(weapon);
	if(shield) free(shield);
	if(armor) free(armor);
	for(i = 0; i < 10; i++)
		if(inventory[i]) free(inventory[i]);
	exit(1);
}

void loop()
{
	char c;

	while(42) {
		if(turn_spent)
			m_move();

		display_map();

		if(turn_spent) {
			turn++;
			m_fight();
			regain_hp_in--;
			if(regain_hp_in == 0) {
				if(rodney.hp < rodney.max_hp)
					rodney.hp++;
				regain_hp_in = 5;
			}
			chk_dead("something (this is a BUG)");
		}
		turn_spent = 0;
		if(lvl_nb == FINAL_LVL && !find_yendor())
			win();

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
			break;
		case 'O':
			show_objects();
			break;
		case 'W':
			wish();
			break;
		case 'A':
			add_rat();
			break;
		case 'E': //Eradicate
			free_monsters(m_list);
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
		case 'b':
			bow_display();	//bow() wrapper
			break;
		}

		while((c = getchar()) != '\n'); // Flush stdin
	}
}

int main(void)
{
	int seed = time(NULL);

	srand(seed);
	fprintf(stderr, "seed = %d\n", seed);
	printf("OPMORL-alpha revision " REVISION "\n");
	first_init();
	fill_map();

	check_visit();

	loop();
	
	clean_exit(0);
	 
	save();
	return 1;
}
