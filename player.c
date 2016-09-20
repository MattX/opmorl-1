/*
 * player.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 *      Owner : both
 *	Copyright : Opmorl, inc.
 */

#include "opmorl.h"

#define V_NO 0
#define V_YES 1
#define V_COMBAT 2

int val_pos(int x, int y)
{
	if(x < 0 || x > 10 || y < 0 || y > 21 ||
			lvl_map[x][y] == T_WALL || lvl_map[x][y] == T_NONE)
		return V_NO;
	if(get_monster(x, y) != NULL)
		return V_COMBAT;
	return V_YES;
}

int is_floor(int x, int y)
{
	if(x <= 0 || x >= 11 || y <= 0 || y >= 21) return 0;
	if(lvl_map[x][y] == T_FLOOR || lvl_map[x][y] == T_STAIRS) return 1;
	return 0;
}

/** find_walls : find edges (walls) of the room.
 * @param w_up upper wall
 * @param w_down lower wall
 * @param w_left left wall
 * @param w_right right wall
 * @note this function is a bit buggy. //In which way ?
 */

void find_walls(int * w_up, int * w_down, int * w_left, int * w_right)
{
	int cx = rodney.posx, cy = rodney.posy;

	//Find left & right walls
	while(is_floor(cx, --cy));
	*w_left = cy;
	while(is_floor(cx, ++cy));
	*w_right = cy;
	cy = rodney.posy;

	//Find up & down walls
	while(is_floor(--cx, cy));
	*w_up = cx;
	while(is_floor(++cx, cy));
	*w_down = cx;
}


void check_visit()
{
	int i, j;
	int w_up, w_down, w_left, w_right;

	//Backup seen areas
	for(i = 0; i < 12; i++)
		for(j = 0; j < 22; j++)
			if(map_status[i][j] == TS_SEEN) map_status[i][j] = TS_DARK;

	//If in a corridor
	if(lvl_map[rodney.posx][rodney.posy] == T_CORRIDOR) { 
		for(i = rodney.posx-1; i <= rodney.posx+1; i++)
			for(j = rodney.posy-1; j <= rodney.posy+1; j++) {
				if(i < 0 || i > 11 || j < 0 || j > 21) continue;
				map_status[i][j] = TS_SEEN;
				if(get_monster(i,j) != NULL)
					get_monster(i,j)->awake=1;
			}
		return;
	}

	//If in a room
	if(lvl_map[rodney.posx][rodney.posy] == T_FLOOR ||
			lvl_map[rodney.posx][rodney.posy] == T_STAIRS) {
		find_walls(&w_up, &w_down, &w_left, &w_right);
		for(i = w_up; i <= w_down; i++)
			for(j = w_left; j <= w_right; j++) {
				map_status[i][j] = TS_SEEN;
				if(get_monster(i,j) != NULL)
					get_monster(i,j)->awake=1;
			}
	}
}

#ifdef DEBUG
void fill_visit()
{
	int i, j;
	for(i = 0; i < 12; i++)
		for(j = 0; j < 22; j++)
			map_status[i][j] = TS_SEEN;
}
#endif

void move_letter(char c)
{
	int ret, mon; //Whether the monster's dead

	switch(c) {
	case 'h': // move <-
		ret = val_pos(rodney.posx, rodney.posy-1);

		if(!ret) return;
		if(ret == V_COMBAT) {
			mon = p_fight(rodney.posx, rodney.posy-1);
			break; //Can't move if you fight
		}
		rodney.posy--; 
		break;
	case 'j': // move v
		ret = val_pos(rodney.posx+1, rodney.posy);

		if(!ret) return;
		if(ret == V_COMBAT) {
			mon = p_fight(rodney.posx+1, rodney.posy);
			break;
		}
		rodney.posx++;
		break;
	case 'k': // move ^
		ret = val_pos(rodney.posx-1, rodney.posy);

		if(!ret) return;
		if(ret == V_COMBAT) {
			mon = p_fight(rodney.posx-1, rodney.posy);
			break;
		}
		rodney.posx--;
		break;
	case 'l': // move ->
		ret = val_pos(rodney.posx, rodney.posy+1);

		if(!ret) return;
		if(ret == V_COMBAT) {
			mon = p_fight(rodney.posx, rodney.posy+1);
			break;
		}
		rodney.posy++;
		break;

	}
	
	if (mon == 1) //This is weird, we should change p_fight.
		display_msg("You killed the monster");
	check_visit();
}


void drop() {
	int i = 0, index;
	char fail[] = "\nYou cannot drop here.\n";

	while (inventory[i] != NULL) {
		printf("%d. %s\n", i, inventory[i]->name);
		if (inventory[i]->class == C_WAND)
			printf("\t%d shots remaining\n", inventory[i]->shots_left);
		i++;
	}
	if (weapon != NULL) printf("10. equipped weapon : %s\n", weapon->name);
	if (shield != NULL) printf("11. equipped shield : %s\n", shield->name);
	if (armor  != NULL) printf("12. equipped armor : %s\n",  armor->name);
	printf("Which object do you want to drop ?");
	scanf("%d", &index);
	if (drop_object(index) == 0)	//For some unknown reason, drop() returns 0 if success (?) /* No, it returns 1 N00B */
		display_msg(fail);
}

void display_man() {
	printf("h, j, k, l : move\n"
		   "q : save'n'quit\n"
		   "> : when on stairs (>), get down the next level\n"
		   "d : drop\n"
		   ", : loot\n"
		   "z : zap\n"
		   "b : drink\n" 
		   "m : the man. Readable once a game.\n");
}

void man() {
	static int nb = 0;
	if (!nb)
		display_man();
	else
		switch (nb) {
			case 1:
				printf("Didn't I tell you the man is ONCE only ?\n");
				break;
			case 2:
				printf("You wanna read it once more, don't you, n00b ?\n");
				break;
			case 3:
				printf("Well, OK, because it's you. But stop pissing me.\n");
				display_man();
				break;
			case 4:
				printf("Fuck off.\n");
				break;
			case 5:
				printf("The next time you try this, you die.\n");
				break;
			case 6:
				printf("You don't believe me ? You don't believe in my ultimate power ?"
					   "I am Buddha, I do whatever I want. Including displaying the man.\n");
				display_man();
				break;
			case 7:
				printf("You're dead.\n");
				break;
			case 8:
				printf("Did you believe this time ? I won't answer again.\n");
				break;
			case 9:
				printf("You want a confirmation ? Go to New Zealand's baboon paradise to see if I'm there.\n");
				break;
			case 10:
				printf("Found me up there ? N00b. Now I will sleep for ten years.\n");
				break;
			case 50:
				printf("You woke me up. Die. And learn the man.\n");
				display_man();
				clean_exit(0);
				break;
		}
	nb++;
}

void display_state() {
	int i;
	if (weapon) printf("Weapon : a %s\n", weapon->name);
	printf("Arrows	: %d\n", rodney.arrows);
	printf("Gold	: %d\n", rodney.gold);
	printf("Stats	: HP %d/%d, LVL %d (%d)\n", rodney.hp, rodney.max_hp,
			rodney.exp_lvl, rodney.exp_b);
	for (i = 0; i < 10; i++)
		if (inventory[i])
			printf("A %s.\n", inventory[i]->name);
	putchar('\n');
}

void open_door() { //This function fails, the two southern doors acceed the school (which fails, too)
	if (lvl_map[rodney.posx][rodney.posy] == T_DOOR) { 
		if (rodney.posx == 3 && rodney.posy == 5)			//North West
			shop();
		else if (rodney.posx == 3 && rodney.posy == 17)		//North East
			spt();

		else if (rodney.posy == 7 && rodney.posy == 5)		//South West
			castle();
		else												//South East
			special();
 
	}
}

void check_exp_lvl() {
	if(rodney.exp_b >= rodney.exp_lvl * 10) {
		rodney.exp_b = 0;
		rodney.exp_lvl++;
		rodney.max_hp += 4;
		rodney.hp = rodney.hp / (rodney.max_hp-4) * rodney.max_hp;
	}
}
