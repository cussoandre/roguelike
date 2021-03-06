#include <iostream>
#include <string>
#include <ncurses.h>

#define WIDTH 64
#define HEIGHT 32

#define XOFF 20
#define YOFF 10

#define MAX_INVENTORY 32
#define MAXHP 400

#define HORIZWALL '_'
#define VERTWALL '|'
#define EMPTY ' '

#define OBSTACLE_A 'X'
#define OBSTACLE_B 'Z'
#define OBSTACLE_C 'O'

#define NUMTILES 7

#define ITEM_A '#'
#define ITEM_A_NAME "Potion"

#define PLAYER '@'

#define OBST 'O'
#define ITEM 'I'
#define FLOOR 'F'
#define WALL 'X'
#define COIN '*'
#define TRIG '+'

#define PLYRFLD 5

int seed = 42;

char tiles[NUMTILES] = {COIN, COIN, COIN, OBSTACLE_A, OBSTACLE_B, OBSTACLE_C, ITEM_A};
char tileTypes[NUMTILES] = {COIN, COIN, COIN, OBST, OBST, OBST, ITEM};

struct itemList {
	char item;
	itemList *prev = NULL;
	itemList *next = NULL;
};

typedef itemList *itmlsp;

struct player {
	int posY, posX;
	int hp = 100;
	int mana = 100;
	int points = 0;
	char id = '@';
	char items[MAX_INVENTORY];
};

typedef player *plyrPnt;

struct map {
	char typemap[HEIGHT][WIDTH];
	char charmap[HEIGHT][WIDTH];
};

map emptyMap(int h, int w)
{
	map newMap;
	
	for (int i = 1; i < h-1; i++)
	{
		for (int j = 1; j < w-1; j++)
		{
			newMap.charmap[i][j] = EMPTY;
			newMap.typemap[i][j] = FLOOR;
		}


		newMap.charmap[i][0] = VERTWALL;
		newMap.charmap[i][w-1] = VERTWALL;

		newMap.typemap[i][0] = WALL;
		newMap.typemap[i][w-1] = WALL;
	}
	for (int j = 0; j < w; j++ )
	{

		newMap.typemap[0][j] = WALL;
		newMap.typemap[h-1][j] = WALL;
		newMap.charmap[0][j] = HORIZWALL;
		newMap.charmap[h-1][j] = HORIZWALL;
	}

	newMap.charmap[h-1][w/2] = EMPTY;
	newMap.typemap[h-1][w/2] = TRIG;
	
	newMap.charmap[h/2][0] = EMPTY;
	newMap.typemap[h/2][0] = TRIG;
	
	newMap.charmap[0][w/2] = EMPTY;
	newMap.typemap[0][w/2] = TRIG;
	
	newMap.charmap[h/2][w-1] = EMPTY;
	newMap.typemap[h/2][w-1] = TRIG;
	return newMap;
}

void prntMap (int h, int w, int XOffset, int YOffset, map toBePrinted, player p)
{

	start_color();
	init_pair(1, COLOR_BLACK, COLOR_RED); //Lava (X obstacle)
	init_pair(2, COLOR_MAGENTA, COLOR_CYAN); //Player
	init_pair(3, COLOR_BLACK, COLOR_BLACK); //Pavement
	init_pair(4, COLOR_CYAN, COLOR_YELLOW); //Coin
	init_pair(5, COLOR_WHITE, COLOR_BLUE);//Obstacle non-damaging
	init_pair(6, COLOR_RED, COLOR_WHITE); //Wall
	init_pair(7, COLOR_WHITE, COLOR_MAGENTA); //Potion

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if ((p.posX == j) && (p.posY == i))
			{
				attron(COLOR_PAIR(2));
				mvaddch(i + YOffset, j + XOffset, p.id);
				attroff(COLOR_PAIR(2));
			}
			else 
			{
				switch (toBePrinted.charmap[i][j])
				{
					case OBSTACLE_A:
						attron(COLOR_PAIR(1));
						
						
						mvaddch(i + YOffset, j + XOffset, toBePrinted.charmap[i][j]);
						attroff(COLOR_PAIR(1));
						break;
					case OBSTACLE_B:
						attron(COLOR_PAIR(5));
						
						
						mvaddch(i + YOffset, j + XOffset, toBePrinted.charmap[i][j]);
						attroff(COLOR_PAIR(5));
						break;
					case OBSTACLE_C:
						attron(COLOR_PAIR(5));
						
						mvaddch(i + YOffset, j + XOffset, toBePrinted.charmap[i][j]);
						attroff(COLOR_PAIR(5));
						break;
					case COIN:
						attron(COLOR_PAIR(4));
						
						mvaddch(i + YOffset, j + XOffset, toBePrinted.charmap[i][j]);
						attroff(COLOR_PAIR(4));
						break;
					case EMPTY:
						attron(COLOR_PAIR(3));
						
						mvaddch(i + YOffset, j + XOffset, toBePrinted.charmap[i][j]);
						attroff(COLOR_PAIR(3));
						break;
					case VERTWALL:
						attron(COLOR_PAIR(6));
						
						mvaddch(i + YOffset, j + XOffset, toBePrinted.charmap[i][j]);
						attroff(COLOR_PAIR(6));
						break;
					case HORIZWALL:
						attron(COLOR_PAIR(6));
						
						mvaddch(i + YOffset, j + XOffset, toBePrinted.charmap[i][j]);
						attroff(COLOR_PAIR(6));
						break;

					case ITEM_A:
						attron(COLOR_PAIR(7));
						
						mvaddch(i + YOffset, j + XOffset, toBePrinted.charmap[i][j]);
						attroff(COLOR_PAIR(7));
						break;
				}
			}
		}
	}
}

map generateRandomMap(int seed, int chanceOffset, map myMap)
{
	srand(seed);
	//map myMap;
	int random;
	for (int i = 1; i < HEIGHT-1; i++)
	{
		for (int j = 1; j < WIDTH-1; j++)
		{	
			random = rand()%(NUMTILES+chanceOffset);

			
			if (random < NUMTILES)
			{
				myMap.charmap[i][j] = tiles[random];
				myMap.typemap[i][j] = tileTypes[random];
			}		
			else
			{
				myMap.charmap[i][j] = EMPTY;
				myMap.typemap[i][j] = FLOOR;
			}		
		}
	}
	return myMap;
}

map updateMap(map currentMap, int playerX, int playerY)
{
	return currentMap;
}

void addItem (plyrPnt p, char itm)
{	
	for (int i = 0; i < MAX_INVENTORY; i++)
	{
		if (p->items[i]==0)
		{
			p->items[i] = itm;
			break;
		}
	}
}

bool checkCollInter (plyrPnt p, map m, int newX, int newY)
{	
	char newPos = m.charmap[newY][newX];
	char nPType = m.typemap[newY][newX];

	if (nPType == FLOOR)
	{
		return true;
	}
	else if (nPType == ITEM)
	{
		addItem(p, newPos);
		return true;
	}
	else if (nPType == OBST)
	{
		if (newPos == OBSTACLE_A) p->hp -=10;
		return false;
	}
	else if (nPType == COIN)
	{
		p->points++;
		return true;
	}
	else if (nPType == TRIG)
	{
		return true;
	}
	else if (nPType == WALL) return false;
	return false;
}

void drawStats (player	p)
{
	move (0,0);
	int baseX = WIDTH + (XOFF * 1.2);

//	move (YOFF, baseX);
	
	char bufHP[4], bufMana[4], bufPT[6]; // need a buffer for that
	 
	sprintf(bufHP,"%d",p.hp);
 	const char* c_hp = bufHP;
	sprintf(bufMana, "%d", p.mana);
	const char* c_mana = bufMana;
	sprintf(bufPT, "%d", p.points);
	const char* c_points = bufPT;	

	printw ("\nPoints:\t");
	printw(c_points);
	printw("\nHealth:\t");
	printw (c_hp);
	printw("\nMana:\t");
	printw (c_mana);



	printw("\nItems:\n");
	char item;
	for(int i = 0; i < MAX_INVENTORY; i++)
	{
		item = p.items[i];
		if (item == ITEM_A) printw(ITEM_A_NAME);
		printw("\n");
	}
}

void useItem (plyrPnt p, char itm)
{
	for (int i = 0; i < MAX_INVENTORY; i++)
	{	
		if (p->items[i] == itm)
		{
			switch (itm)
			{
				case ITEM_A:
					p->hp += 5;
					break;
			}
			p->items[i] = 0;
			break;
		}
	}
}

void loseScreen (player p)
{
	move (0,0);
	clear();
	init_pair(9, COLOR_BLACK, COLOR_RED);
	attron(COLOR_PAIR(9));
	printw ("\n\n\nYOU DIED.\nThese were your stats\n\n\n");
	drawStats(p);
	attroff(COLOR_PAIR(9));
	refresh();
	cbreak();
	while(getch()!='X');
	endwin();
	exit(0);
}

int main()
{
	initscr();
	if (has_colors() == false)
	{
		printw("this version of ROGUELIKE needs a color terminal to work");
		refresh();
		getch();
		endwin();
		exit(1);
	}

	raw();
//	map myMap;
	map gameMap;
	char input;
	player P1;

	int x2, y2;

	P1.posX = 6;
	P1.posY = 3;

	gameMap = generateRandomMap(seed, 60, emptyMap(HEIGHT, WIDTH));
	printw("\n:::::::..         ...         .,-:::::/     ...    :::  .,::::::     :::       :::   :::  .     .,::::::  \n;;;;``;;;;     .;;;;;;;.    ,;;-'````'      ;;     ;;;  ;;;;''''     ;;;       ;;;   ;;; .;;,.  ;;;;''''  \n  [[[,/[[['    ,[[     \[[,  [[[   [[[[[[/  [['     [[[   [[cccc      [[[       [[[   [[[[[/'     [[cccc   \n $$$$$$c      $$$,     $$$  \"$$c.    \"$$   $$      $$$   $$\"\"\"\"      $$'       $$$  _$$$$,       $$""""   \n 888b \"88bo,  \"888,_ _,88P   `Y8bo,,,o88o  88    .d888   888oo,__   o88oo,.__  888  \"888\"88o,    888oo,__ \n MMMM   \"W\"     \"YMMMMMP\"      `'YMUP\"YMM   \"YmmMMMM\"\"   \"\"\"\"YUMMM  \"\"\"\"YUMMM  MMM   MMM \"MMP\"   \"\"\"\"YUMMM");
	printw("\n\n\n\t\t\tPRESS ANY KEY TO CONTINUE :)");
	refresh();
	getch();
//	gameMap = myMap;
	while (input != 'X')
	{	
		move(0,0);
		drawStats(P1);		
		x2 = P1.posX;
		y2 = P1.posY;
		//gameMap.charmap[P1.posY][P1.posX] = P1.id;
		prntMap(HEIGHT, WIDTH, XOFF, YOFF, gameMap, P1);
		refresh();
		input = getch();
		
		//gameMap.charmap[P1.posY][P1.posX] = EMPTY;
		switch (input)
		{
			case 'd':
				x2 = P1.posX +1;
				break;

			case 'a':
				x2 = P1.posX -1;
				break;
		
			case 's':
				y2 = P1.posY +1;
				break;
			
			case 'w':
				y2 = P1.posY -1;
				break;
			
			case 'p':
				useItem(&P1, ITEM_A);
				break;	
		}

		//if (p1.posX == 0) && (p1.posY == HEIGHT/2)

		if (P1.posX >= WIDTH-1) P1.posX = WIDTH -2;
		if (P1.posX < 1) P1.posX = 1;
		if (P1.posY >= HEIGHT-1) P1.posY = HEIGHT -2;
		if (P1.posY < 1) P1.posY = 1;
		if (P1.hp > MAXHP) P1.hp = MAXHP;
		if (P1.hp <= 0) loseScreen(P1);
		if (checkCollInter(&P1, gameMap, x2, y2))
		{
			P1.posX = x2;
			P1.posY = y2;
			if ((gameMap.typemap[P1.posY][P1.posX] == ITEM) || (gameMap.typemap[P1.posY][P1.posX] == COIN))	
			{
				gameMap.charmap[P1.posY][P1.posX] = EMPTY;
				gameMap.typemap[P1.posY][P1.posX] = FLOOR;
			}
			else if ((gameMap.typemap[P1.posY][P1.posX]) == TRIG)
			{
				if (P1.posX == 0)
				{
					seed *= 2;
					gameMap = emptyMap(HEIGHT, WIDTH);
					gameMap = generateRandomMap(seed, 30, gameMap);
					P1.posX = WIDTH -2;
				}	
				else if (P1.posX >= WIDTH-1)
				{
					seed /= 2;
					gameMap = emptyMap(HEIGHT, WIDTH);
					gameMap = generateRandomMap(seed, 30, gameMap);

					P1.posX = 0;
				}
				else if (P1.posY == 0)
				{
					seed *= 13;
					gameMap = emptyMap(HEIGHT, WIDTH);
					gameMap = generateRandomMap(seed, 30, gameMap);
						
					P1.posY = HEIGHT -2;
				}	
				else if (P1.posY >= HEIGHT-1)
				{
					seed /= 13;
					gameMap = emptyMap(HEIGHT, WIDTH);
					gameMap = generateRandomMap(seed, 30, gameMap);
					P1.posY = 0;
				}			
			}

		}
	
		
	}
	endwin();
	return 0;
}
