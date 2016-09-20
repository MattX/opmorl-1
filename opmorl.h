/*
 * opmorl.h
 *
 *  Created on: 2 dec. 2009
 *     Authors: zale&ttthebest&buddha
 *	Copyright : Opmorl, inc.
 * 
 *	Special thanks to : nobody yet.
 *
 */

#ifndef OPMORL_H_
#define OPMORL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>


#define FINAL_LVL 25
#define WEAPON_SLOT 10
#define ARMOR_SLOT 11
#define SHIELD_SLOT 12
#define DEBUG
#define REVISION "R11"

typedef enum { T_FLOOR, T_WALL, T_CORRIDOR, T_NONE, T_STAIRS, T_DOOR } Tile;
Tile lvl_map[12][22];
typedef enum { TS_SEEN, TS_DARK, TS_UNVISITED } Tilestat;
Tilestat map_status[12][22];

int lvl_nb;
int turn;
int seed;
int regain_hp_in;

typedef enum {
	C_WAND, C_POTION, C_BOW, C_SWORD, C_ARROW, C_ARMOR_B, C_ARMOR_S, C_GOLD
} Class; /* ARMOR_B : body ; ARMOR_S : shield */

/* A precision on gold : the value is generated between 50 and 100.		*
 * If the object is gotten, do NOT add it to the inventory : increment	*
 * rodney.gold and then immediately remove the object. Displayed by '€'	*/

typedef struct obj_type { 
	Class class; 
	char name[50];
	int posx, posy; 
	int attack; /* For weapons & also shields/armor */
	int target_hp; /* For wands & potions, bonus of life (if > 0) or damage (if < 0) they do to the target */
	int freezes; /* For wands */
	int shots_left; /* For wands */
	int val; /* to buy and sell, and for gold. */
	/* When shots_left == 0 the wand is deleted */
	struct obj_type * next; 
} Object;

Object *inventory[10]; 
Object *weapon;
Object *shield;
Object *armor;

Object *spt_inv[10];

typedef struct {
	int posx, posy;
	int gold;		// We at last use this one, lol.
	int arrows;
	int exp_b;          //Number of shots in general + other factors.
	int bow_b, sword_b; //Number of _shoots_ with bows and swords.
	int exp_lvl;		//Experience level
	int hp, max_hp;
} Player;

Player rodney;

typedef struct m_type {
	int posx, posy;
	char name[50];
	int attack;
	int freezes, blocks_magic;
	int is_invisible;
	int awake;
	int proba;
	int hp;

	struct m_type * next;
} Monster;

extern Monster m_default[14];
Monster * m_list;
Monster yendor;

extern Object o_default[17];

Object * o_list; 

int turn_spent; /* this var is to be set if a turn is spent. */

/* FUNCTIONS */

void fill_map();
void display_map();
void display_msg();
void display_man();
void display_state();
void first_init();
void move_letter(char);
int  fight(int, int); //Returns result of the fight. 0 : nothing, 1 : monster dies, 2 : Rodney dies.
Monster * get_monster(int, int);
Monster * add_monster(Monster, int, int);
void free_monsters(Monster *);
void make_monsters();
void rm_monster(int, int);
void free_objects(Object *);
void new_level();
void clear_status();
#ifdef DEBUG
void show_monsters();
void show_objects();
#endif
void m_fight(); //Both fights return 1 if target killed.
int  p_fight(int, int); //Value return of 2 means there wasn't a monster. 
						//It should however never happen.
void m_move();
void make_town(); //6 following funcs are defined in special.c, they shalt refer to towns.
void open_door(); 
void shop();
void school();
void spt();
void castle();
void special();

void man();

int clean_exit(int);

int min(int,int);
int max(int,int);
int min3(int, int, int);
int max3(int, int, int);
int rnd_max(int,int);
void check_visit();

char choice(char, char, char*);

void check_exp_lvl();
void chk_dead(char*);
int take_gold(int);
void swap(int*, int*);

void mon_move(int, int);
Monster * fmonat(int, int, int, int, int*);
void add_rat();

void save();

#include "objet.h"

#endif /* OPMORL_H_ */
