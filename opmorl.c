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

int min(int a, int b)
{
	return a<b ? a:b;
}

int max(int a, int b)
{
	return a>b ? a:b;
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
	srand(time(NULL));

	first_init();
	fill_map();


	loop();

	return EXIT_SUCCESS;
}
