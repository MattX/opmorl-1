/*
 *  objet.c
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 OPMORL, inc. All rights reserved.
 */

#include "opmorl.h"
#define K 800000000


/* every object is defined w/ a x and y of -1 to be off-map. *
 * They are not to be modified, but to be copied into a custom one */
Object o_default[17] = {
/*		type		name					 x	 y	atk	atk	frz	amm	val	next	*/
/* 0 */	{C_WAND,    "Wand of Death",		-1, -1,  0,	-K,	0,	1,	750,NULL}, /* The Wand of Death doesn't freeze, but instakills, no arrows, one shot only (because it'd be busay)*/
/* 1 */	{C_WAND,    "Wand of Life",			-1, -1,  0,	K,	0,	1,	200,NULL}, /* same as the wand of death, more than one'd be busay */
/* 2 */	{C_WAND,    "Wand of Freeze",		-1, -1,  0,	0,	1,	5,	75,	NULL}, /* 5 shots seem good for this one*/
/* 3 */	{C_WAND,    "Wand of Wounds",		-1, -1,  0,	-30,0,	10,	100,NULL}, /* nothing particular, just-a-comment*/
/* 4 */	{C_POTION,  "Potion of Cure",		-1, -1,  0,	15,	0,	1,	75,	NULL}, /* what a crappy name man*/ /* ZALE : NO FTW */ /* FTW ? fuck the window ? find the way ? */
/* 5 */	{C_POTION,  "Potion of Healing",	-1, -1,  0,	35,	0,	1,	200,NULL}, /* the name was so awful that I changed it*/
/* 6 */	{C_POTION,  "Potion of Poison",		-1, -1,  0,	-20,0,	1,	30,	NULL}, /* This one particularly rocks */
/* 7 */	{C_SWORD,   "Wooden Sword",			-1, -1,  5,	0,	0,	K,	25,	NULL}, /* Basic sword, given at the beginning of the game */
/* 8 */	{C_SWORD,   "Iron Sword",			-1, -1,  7,	0,	0,	K,	300,NULL}, /* This Sword is more powerful */
/* 9 */	{C_SWORD,   "Steel Sword",			-1, -1, 10,	0,	0,	K,	600,NULL}, /* The most powerful sword in-game */
/* 10*/	{C_BOW,	    "Simple Bow",			-1, -1,  5,	0,	0,	K,	350,NULL},
/* 11*/	{C_BOW,	    "Coumpound Bow",		-1, -1, 10,	0,	0,	K,	650,NULL}, /* I do love bows */
/* 12*/	{C_ARROW,   "Pack of 10 Arrows",	-1, -1,  0,	0,	0,	0,	100,NULL}, /* Arrows increment from 10 the "arrows" variable from Rodney.*/
/* 13*/	{C_ARMOR_S, "Shield",				-1, -1,  0,	0,	0,	K,	75,	NULL},
/* 14*/	{C_ARMOR_B, "Leather Armor",		-1, -1,  0,	0,	0,	K,	200,NULL},
/* 15*/	{C_ARMOR_B, "Mithril Mail",			-1, -1,  0,	0,	0,	K,	750,NULL},
/* 16*/ {C_GOLD,	"Some pieces of gold",	-1, -1,	 0,	0,	0,	0,	0,	NULL}}; /* DONE \o/ */ 

Object * add_object(Object obj, int posx, int posy)
{
	Object * current = o_list;
	Object * new = malloc(sizeof(Object));

	*new = obj;
	strcpy(new->name, obj.name);
	if (new->class == C_GOLD)
		obj.val = rnd_max(10, 100);
	if(posx != -1)
		new->posx = posx;
	if(posy != -1)
		new->posy = posy;
	new->next = NULL;

	if(current == NULL)
		return o_list = new;

	while(current->next != NULL)
		current = current->next;

	return current->next = new;
}

void rm_object(int posx, int posy)
{
	Object * current = o_list;
	Object * tmp;

	if(current == NULL) return;

	if(current->posx == posx && current->posy == posy) {
		o_list = current->next;
		free(current);
		return; //see rm_monster in monster.c
	}
	while(current->next != NULL) {
		if(current->next->posx == posx && current->next->posy == posy) {
			tmp = current->next->next;
			free(current->next);
			current->next = tmp;
			break;
		}
		current = current->next;
	}
}

#ifdef DEBUG
void show_objects()
{
	Object * current = o_list;
	while(current != NULL) {
		printf("Object at %d,%d : %s.\n",	current->posx,
											current->posy, 
											current->name);
		current = current->next;
	}
}
#endif


int find_near_free_tile(int * posx, int * posy) {
	int i, j;
	int x = rodney.posx, y = rodney.posy;

	if (!isObject(x, y)) {
		*posx = x; *posy = y;
		return 1;
	} else {
		for (i = x-1; i < x+2; i++)
			for (j = y-1; j < y+2; j++)
				if (!isObject(i, j) && lvl_map[i][j] != T_WALL 
					&& lvl_map[i][j] != T_NONE
					&& lvl_map[i][j] != T_STAIRS) { // Can we drop in stairs ? -- NO
					*posx = i;
					*posy = j;
					return 1;
				}
	}
	return 0; // This should be reached when dropping fails, which means the player is surrounded by objects
}
/* This function is not KNOWN to work, but where could bugs hide in 5 lines of code ? */

int isObject(int posx, int posy) {
	Object *tmp = get_object(posx, posy);
	if (tmp == NULL)
		return 0;
	//There was a bug here.
	return 1;
}


/* Called when a new level is generated. */
/* MUST ABSOLUTELY BE CALLED AS free_objects(o_list); */
void free_objects(Object * obj)
{
	if(obj == NULL) return;
	if(obj->next != NULL)
		free_objects(obj->next);
	free(obj);

	o_list = NULL;
}

/* This function is to be called with an existing object at index i */
int drop_object(int i) {
	int j;

	if(i < 0 || i > 13) return 1;

	if (i < 10) { /* means the object is in the inventory */
		if (inventory[i] == NULL) return 1;
		if (find_near_free_tile(&(inventory[i]->posx), &(inventory[i]->posy))) {
			add_object(*inventory[i], -1, -1);
			free(inventory[i]);
			inventory[i] = NULL;

			for (j = i+1; j < 10; j++)
				inventory[j-1] = inventory[j]; //this should _not_ segfault
			inventory[9] = NULL;

			return 1; // dropping succeeded
		}
		return 0; // FAIL
	}

	else { /* the object is either the equipped weapon, armor or shield */
		if (i == WEAPON_SLOT && weapon != NULL) {
			if (find_near_free_tile(&weapon->posx, &weapon->posy)) {
				add_object(*weapon, -1, -1);
				free(weapon);
				weapon = NULL;
				return 1;
			}
		}

		else if (i == ARMOR_SLOT && armor != NULL) {
			if (find_near_free_tile(&armor->posx, &armor->posy)) {
				add_object(*armor, -1, -1);
				free(armor);
				armor = NULL;
				return 1;
			}
		}

		else if (i == SHIELD_SLOT && shield != NULL) {
			if (find_near_free_tile(&shield->posx, &shield->posy)) {
				add_object(*shield, -1, -1);
				free(shield);
				shield = NULL;
				return 1;
			}
		}
	}

	return 0;
}
		

Object * get_object(int posx, int posy)
{
	Object * current = o_list;
	
	while(current != NULL) {
		if(current->posx == posx && current->posy == posy)
			return current;
		current = current->next;
	}
	
	return NULL;
}

void getObject() {
	int i = -1;
	Object *tmp = get_object(rodney.posx, rodney.posy);
	if (!tmp) return;
	if (tmp->class == C_GOLD) {
		rodney.gold += tmp->val; 
		rm_object(rodney.posx, rodney.posy); //As this line wasn't there you could make infinite gø|∂ ! ®øƒ| !
											 //Man, nobody cares.
		return;
	}
	if (tmp->class == C_ARROW) {
		rodney.arrows += 10;
		rm_object(rodney.posx, rodney.posy);
	}
	while(inventory[++i] != NULL);
	if(i >= 10) {
		printf("Sorry, your inventory is full. You should try dropping something, or dying\n");
		return;
	}
	
	inventory[i] = malloc(sizeof(Object));
	*inventory[i] = *tmp;
}

void equip(int inv_index) //MYSTERIOUS THINGS AROUND HERE. PLEASE DO NOT TOUCH
{
	Object * tmp;
	int slot;

	printf("%d\n", inv_index);
	
	if(inv_index < 0 || inv_index > 9 || inventory[inv_index] == NULL) return; //I actually think this line makes it fail.
	
	switch (inventory[inv_index]->class) {
		case C_SWORD:
		case C_BOW:
			slot = WEAPON_SLOT;
			break;
		case C_ARMOR_B:
			slot = ARMOR_SLOT;
			break;
		case C_ARMOR_S:
			slot = SHIELD_SLOT;
			break;
		default: // I think the lack of default also made it fail.
			return;
	}
	switch(slot) {
	case WEAPON_SLOT:
		tmp = weapon;
		weapon = inventory[inv_index];
		break;
	case ARMOR_SLOT:
		tmp = armor;
		armor = inventory[inv_index];
		break;
	case SHIELD_SLOT:
		tmp = shield;
		shield = inventory[inv_index];
		break;
	}
	
	inventory[inv_index] = tmp;
}

void wish() //Segfault suspicion around here.
{
	int ind;

	printf("What do you wish for (id) ? ");
	scanf("%d", &ind);

	if(ind > 16 || ind < 0) return;

	rm_object(rodney.posx, rodney.posy);
	add_object(o_default[ind], rodney.posx, rodney.posy);
}

/** DIRTY CODE */

void make_objects()
{
	int nb_gen = rnd_max(2, 6);
	int posx, posy;
	int ri; // ri for remind, idiot ! BTW how could I remember, OMG I so rock.

	while(nb_gen--) {
		ri = rnd_max(0, 16);

		printf("Selected object %d for creation, name %s.\n", ri, o_default[ri].name);
		do {
			posx = rnd_max(0, 10);
			posy = rnd_max(0, 21);
		} while(get_object(posx, posy) != NULL ||
				lvl_map[posx][posy] == T_WALL || lvl_map[posx][posy] == T_NONE ||
				(rodney.posx == posx && rodney.posy == posy));
		add_object(o_default[ri], posx, posy);
	}
}

/** DIRTY CODE ENDS HERE, FOR GOD'S SAKE */

void bow (int x, int y) {
	if (x < 0 || x >= 12 ||
		y < 0 || y >= 22) 
		return;
	Monster * mon = get_monster(x, y);
	if (mon == NULL) return;
	mon->hp -= weapon->attack;
	rodney.arrows--;
	printf("You shot the monster for %d damage", weapon->attack);
	if (mon->hp < 1) {
		printf(" and it died.\n");
		rm_object(x, y);
	}
	else 
		printf(", it has %d HP remaining.\n", mon->hp);
	printf("You have %d arrow(s) left.\n", rodney.arrows);
}

void zap(int x, int y, int index)	/* the index of the wand in the inventory */ 			
{									//TODO: Fix this apparently not working func. 
	/*
	 * The code in here should be *very* complicated. Not just like 15 lines.
	 */
	if (x < 0 || x >= 12 ||
		y < 0 || y >= 22) 
		return;
	Monster *mon = get_monster(x, y);
	if (mon == NULL) return;
	if (inventory[index]->shots_left > 0) {
		mon->hp += inventory[index]->target_hp; /* The target_hp is negative -> health is removed, therefore + */
		inventory[index]->shots_left--;
		printf("The monster lost %d HP", inventory[index]->target_hp);
	}
	if (mon->hp < 1) {
		printf(" and died.\n");
		rm_monster(x, y);
	}
	else 
		printf(" and has %d remaining.\n", mon->hp);
}

void zap_display() {
	int i, is, wand, x, y;
	for (i = 0; i < 10; i++)
		if (inventory[i])
			if (inventory[i]->class == C_WAND) {
				printf("%d. %s with %d shot(s) left.\n", i, inventory[i]->name, inventory[i]->shots_left);
				is++;
			}
	if (is) {
		printf("Which wand to use ? ");
		scanf("%d", &wand);
		if (!inventory[wand]) return;
		if (inventory[wand]->class != C_WAND)  {
			printf("This is not a wand.\n");
			return;
		}
		printf("Which coordonates to zap ?\n");
		scanf("%d%d", &x, &y);
		zap(x, y, wand); //The zap func should check whether the coordonates are valid.
	}
	else
		printf("There is no wand to use.\n");
	return;
	
}

void equip_display() {
	int i, index;

	if(!inventory[0]) {
		printf("You can't equip anything.\n");
		return;
	}
	for (i = 0; i < 10; i++) 
		if (inventory[i])
			if (inventory[i]->class == C_ARMOR_B ||
				inventory[i]->class == C_ARMOR_S ||
				inventory[i]->class == C_BOW	 ||
				inventory[i]->class == C_SWORD)
				printf("%d. A %s\n", i, inventory[i]->name);
	scanf("%d", &index);
	equip(index); //IS not functional.
}

void bow_display() {
	int x, y;
	if (!weapon){
		printf("You don't have a weapon.\n");
		return;
	}
	else {
		if (weapon->class != C_BOW) {
			printf("Your weapon is not a bow. Please equip one.");
			if (!rodney.arrows)
				printf(" OBTW, you have no arrows left.\n");
			else 
				printf(" You have anyway %d arrows left.\n");
			return;
		}
		else {
			printf("You have a %s, with %d arrows.\n", weapon->name, rodney.arrows);
			printf("Where to shoot ?\n");
			scanf("%d%d", &x, &y);
			bow(x, y);
		}

	}
	
}

void drink() {
	int i, is = 0, potion;
	for (i = 0; i < 10; i++)
		if (inventory[i])
			if (inventory[i]->class == C_POTION) {
				printf("%d. A %s\n", i, inventory[i]->name);
				is++;
			}
	if (!is) {
		printf("You don't have any potion.");
		return;
	}
	else {
		printf("Which one do you want to drink ?\n");
		scanf("%d", &potion);
	}
	if (inventory[potion]->class != C_POTION) {
		printf("This is no potion, sucker.\n");
		return;
	}
	rodney.hp+= inventory[potion]->target_hp;
	if (inventory[potion]->target_hp > 0)
		printf("You heal %d HP.\n", inventory[potion]->target_hp);
	if (inventory[potion]->target_hp < 0 && rodney.hp > 0)
		printf("You lose %d HP.\n", -inventory[potion]->target_hp);
	free(inventory[potion]);
	for (i = potion+1; i < 10; i++)
		inventory[i-1] = inventory[i];
	inventory[9] = NULL;
		//TODO: add fucking chk_dead()
	if (rodney.hp < 1) {
		printf("You die, poisoned by a potion.");
		clean_exit(0);
	}

}

//TODO: replace all tests on object index by this function.
Object * amgo(int index) // AutoMagic Get Object
{
	if(index < 0 || index > 12)
		return NULL;
	else if(index < 10) //This may return NULL, use with caution.
		return inventory[index];
	else if(index == 10)
		return weapon;
	else if(index == 11)
		return armor;
	else
		return shield;
}

void just_dropped(int index)
{
	int i;

	if(index < 0 || index >= 10)
		return;

	free(inventory[index]); //You had forgotten this ! On an iP{ad|od|hone} w/ ~60 Mo of RAM for you, we can't permit it.
	for (i = index+1; i < 10; i++)
		inventory[i-1] = inventory[i];
	inventory[9] = NULL;
}
