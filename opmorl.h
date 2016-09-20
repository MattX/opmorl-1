/*
 * opmorl.h
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 */

#ifndef OPMORL_H_
#define OPMORL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define FINAL_LVL 20
#define DEBUG 1

typedef enum { T_FLOOR, T_WALL, T_CORRIDOR, T_NONE, T_STAIRS } Tile;
Tile lvl_map[12][11];

int going_up;
int lvl_nb;
int turn;

typedef struct {
	int posx, posy;
	int gold;
	int sword_b, bow_b;
	int exp_b;
	int hp;
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

Monster m_default[13];
Monster * m_list;

/* FUNCTIONS */

void fill_map();
void display_map();
void display_msg();
void first_init();
void move_letter(char c);
void fight(Monster * mon);
Monster * add_monster(Monster mon, int posx, int posy);
void free_monsters(Monster * mon);
void make_monsters();

int min(int,int);
int max(int,int);
int rnd_max(int,int);

#endif /* OPMORL_H_ */
