//Paul A. Fischer, Don Jervis
//Computer Science II, Perry Kivolowitz
//Final Project, 4/27/17-5/9/17
//Tanks

// (up and down, left and right)
//stringstream cannot use curses/ncurses operations
//use stringstream for c++
//use addch and char* for ncurses
//game is 40 lines
//game is 120 cols

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cmath>
#include <Windows.h>
#include <locale.h>
#include <conio.h>

#if defined(WIN32)
#include "curses.h"
#include "ground.hpp"
#include "player.hpp"
#else
#include <curses.h>
#include <ground.hpp>
#include <player.hpp>
#endif

using namespace std;


extern int lines;					//extern means there is a global variable somewhere, but it isn't located here (located in ground.cpp)
extern int cols;
extern int base_height_divisor;
extern int max_height_divisor;
int gas_toggle = 5;
int health_toggle = 3;
int color_toggle_one = 0;
int color_toggle_two = 0;
char temp_nickname_one[24];
bool nickname_check_one = false;
char temp_nickname_two[24];
bool nickname_check_two = false;
bool destroy = false;
bool land = false;
bool butter = false;
bool ft = false;

int p1s = 0;						//player 1 score storage
int p2s = 0;						//player 2 score storage
int ground_type = 2;
bool show = false;					//show debugging stuff

bool wind_change = false;			//done in the settings
int wind_level = 0;

const double PI = 3.141592653589793238463;




//this draws the game onto the screen so we can see it
void DrawScreen(Ground & g, Player * players, int turn)
{
	erase();
	box(stdscr, 0, 0);
	g.Draw();
	players[0].Draw(g);
	players[1].Draw(g);
	players[0].DrawSettings(turn);
	players[1].DrawSettings(turn);
	refresh();
}

//http://www.iforce2d.net/b2dtut/projected-trajectory

//this function shoots bullet
//projects the path of the bullet as ACS_BULLET(before was *)
//it wants to know where the ground is, where the player is, and whos turn it is
//if bullet goes far to the right it stops doing ground damage at a certain col


void Wind()
{
	int wc2c = rand() % 10;		//wind chance to change
	bool first_turn = true;
	//this gives it a chance to stay the same for more than one turn.

	if (wind_change == true)
	{
		if (first_turn == true)
		{
			int r = rand() % 6;
			//low wind left
			if (r == 0)
			{
				wind_level = 1;
			}

			//medium wind left 
			else if (r == 1)
			{
				wind_level = 2;
			}

			//strong wind left
			else if (r == 2)
			{
				wind_level = 3;
			}

			//low wind right
			else if (r == 3)
			{
				wind_level = 4;
			}

			//medium wind tight
			else if (r == 4)
			{
				wind_level = 5;
			}

			//strong wind right
			else if (r == 5)
			{
				wind_level = 6;
			}
			first_turn = false;
		}

		else if (wc2c > 3)
		{
			int r = rand() % 6;
			//low wind left
			if (r == 0)
			{
				wind_level = 1;
			}

			//medium wind left 
			else if (r == 1)
			{
				wind_level = 2;
			}

			//strong wind left
			else if (r == 2)
			{
				wind_level = 3;
			}

			//low wind right
			else if (r == 3)
			{
				wind_level = 4;
			}

			//medium wind tight
			else if (r == 4)
			{
				wind_level = 5;
			}

			//strong wind right
			else if (r == 5)
			{
				wind_level = 6;
			}
		}
	}

	else
	{
		//else wind is false and default of .2 is shown
		wind_level = 0;
	}
}


void Shoot(Ground & g, Player * players, int turn, int bih, int biv)
{

	players[turn].ws = 0.0; //resets it

							//			no wind
	if (wind_level == 0)
		players[turn].ws = .200;

	if (players[turn].s == LEFT)					// >>> then the wind goes with shot if facing right, against if facing left
	{
		//			<<<
		if (wind_level == 1)			//low left
			players[0].ws = 0.180;
		else if (wind_level == 2)		//medium left
			players[0].ws = 0.155;
		else if (wind_level == 3)		//high left
			players[0].ws = 0.125;
		//			>>>
		if (wind_level == 4)			//low right
			players[0].ws = 0.220;
		else if (wind_level == 5)		//medium right
			players[0].ws = 0.235;
		else if (wind_level == 6)		//high right
			players[0].ws = 0.275;
	}

	else if (players[turn].s == RIGHT)				// >>> then the wind goes with shot if facing left, against if facing right
	{
		//			<<<
		if (wind_level == 1)			//low left
			players[1].ws = 0.220;
		else if (wind_level == 2)		//medium left
			players[1].ws = 0.235;
		else if (wind_level == 3)		//high left
			players[1].ws = 0.275;
		//			>>>
		if (wind_level == 4)			//low right
			players[1].ws = 0.180;
		else if (wind_level == 5)		//medium right
			players[1].ws = 0.155;
		else if (wind_level == 6)		//high right
			players[1].ws = 0.125;
	}


	start_color();
	init_pair(0, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);

	//this takes the angle and turns it into radians
	double angle = players[turn].angle / 180.0 * PI;


	//double y_component = sin(angle) * players[turn].power * 0.2;
	//double x_component = cos(angle) * players[turn].power * 0.2;

	//vertical
	double y_component = (sin(angle) * players[turn].power * 0.2);
	//horizontal
	double x_component = (cos(angle) * players[turn].power * players[turn].ws);

	//creating 2 new doubles, 
	double pNx;
	double pNy;

	//flips the bullet if it's the player on the other side of the screen
	if (players[turn].s == RIGHT)
		x_component = -x_component;

	double p0x = players[turn].col;
	double p0y = g.ground.at(players[turn].col);
	// higher ground numbers are lower altitudes (0 is first line, etc).

	p0y = lines - p0y;


	for (int i = 1; i < 5000; i++)
	{
		double di = i / 5.0;																					//<< double di has to do with the speed of the bullet

																												//new position of the bomb = old position of the bomb + di times the formula for that players shot
		pNx = (int)(p0x + di * x_component);
		//this is the speed of the bullet depending on what line it is at
		pNy = p0y + di * y_component + (di * di + di) * -0.98 / 2.0;
		pNy = (int)(lines - pNy);
		//if it goes too far left or too far right this ends the turn
		if (pNx < 1 || pNx >= cols - 2)
			break;
		//if this goes off the screen upwards it just sleeps until the bomb comes back in
		if (pNy < 1) {
			Sleep(50);
			continue;
		}

		//if bullet goes off the screen at the bottom
		//if (pNy >= lines - 2)
		//break;

		//if the bullet is in the ground then break
		//giving the ground error. if it's off the screen or on the last one it cannot break the ground off the screen
		if (pNy >= g.ground.at((int)pNx))
		{
			destroy = true;
			break;
		}

		//if you directly hit the tank
		if (pNy == players[turn].col && pNx == players[turn].line)
		{
			break;
		}


		//this makes the bullet only one
		erase();
		DrawScreen(g, players, turn);

		if (show == true)
		{
			stringstream ss;
			ss = stringstream();
			ss << setw(3) << right << pNx;
			move(LINES - 1, COLS / 2 - 3);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << pNy;
			move(LINES - 1, COLS / 2 + 8);
			addstr(ss.str().c_str());
			refresh();
		}

		move((int)pNy - 1, (int)pNx + 1);

		attron(COLOR_PAIR(players[turn].bomb_type));
		//make bomb the color of the bombtype
		addch(ACS_BULLET);

		attroff(COLOR_PAIR(players[turn].bomb_type));

		refresh();
		Sleep(50);
	}

	//h is up and down
	//v is left and right
	bih = pNx;
	biv = pNy - 1;

	if (destroy == true)
	{//if it's in the range of the game
		if (biv > 1 || biv < COLS - 2)
		{
			g.ground.at(bih)++;
		}

		//if it's on screen and there is at least one more space
		if (biv > 2)
		{
			g.ground.at(bih - 1)++;
		}
		//if it's on screen and there is at least one more space
		if (biv < COLS - 4)
		{
			g.ground.at(bih + 1)++;
		}

		//if it's a large bomb- radius is bigger
		if (players[turn].bomb_type == 2)
		{
			if (biv > 5)
			{
				g.ground.at(bih - 2)++;
			}
			if (biv < COLS - 5)
			{
				g.ground.at(bih + 2)++;
			}
		}
		destroy = false;
	}

	attron(COLOR_PAIR(players[turn].bomb_type));

	stringstream ss;
	ss << "#";
	move(biv, bih + 1);
	addstr(ss.str().c_str());
	refresh();

	attroff(COLOR_PAIR(players[turn].bomb_type));

	Sleep(1200);

	/////////////////////////////////////////BOMB_TYPE_0//////////////////////////////////////////////
	//regular bomb bomb_type == 0
	//this does a regular bomb
	if (players[turn].bomb_type == 0)
	{
		//if bomb is within 1 column in either direction of player 1
		if (bih == players[0].col || bih == players[0].col + 1 || bih == players[0].col + 2)
		{
			//if bomb is within 1 row in either direction of player 1
			if (biv == players[0].line || biv == players[0].line + 1 || biv == players[0].line - 1)
			{
				players[1].points = players[1].points + 25;
				players[0].health--;
			}
		}

		//if bomb is within 1 column in either direction of player 2
		if (bih == players[1].col || bih == players[1].col + 1 || bih == players[1].col + 2)
		{
			if (biv == players[1].line || biv == players[1].line + 1 || biv == players[1].line - 1)
			{
				players[0].points = players[0].points + 25;
				players[1].health--;
			}
		}
		players[turn].bomb_type = 0;
	}

	/////////////////////////////////////////BOMB_TYPE_1//////////////////////////////////////////////

	//this does twice the damage

	else if (players[turn].bomb_type == 1)
	{
		if (bih == players[0].col || bih == players[0].col + 1 || bih == players[0].col + 2)
		{
			if (biv == players[0].line || biv == players[0].line + 1 || biv == players[0].line - 1)
			{
				players[1].points = players[1].points + 50;
				players[0].health = players[0].health - 2;
			}
		}

		//if bomb is within 1 column in either direction of player 2
		if (bih == players[1].col || bih == players[1].col + 1 || bih == players[1].col + 2)
		{
			if (biv == players[1].line || biv == players[1].line + 1 || biv == players[1].line - 1)
			{
				players[0].points = players[0].points + 50;
				players[1].health = players[1].health - 2;
			}
		}
		players[turn].bomb_type = 0;
	}

	/////////////////////////////////////////BOMB_TYPE_2//////////////////////////////////////////////

	//if bomb is within 2 columns to the left or right, or 2 rows up or down
	//large bomb bomb (bigger radius) = 2

	else if (players[turn].bomb_type == 2)
	{
		if (bih == players[0].col || bih == players[0].col + 1 || bih == players[0].col + 2 || bih == players[0].col - 1 || bih == players[0].col + 3)
		{
			if (biv == players[0].line || biv == players[0].line + 1 || biv == players[0].line + 2 || biv == players[0].line - 1 || biv == players[0].line - 2)
			{
				players[1].points = players[1].points + 25;
				players[0].health--;
			}
		}

		//if bomb is within 1 column in either direction of player 2
		if (bih == players[1].col || bih == players[1].col + 1 || bih == players[1].col + 2 || bih == players[1].col - 1 || bih == players[1].col + 3)
		{
			if (biv == players[1].line || biv == players[1].line + 1 || biv == players[1].line + 2 || biv == players[1].line - 1 || biv == players[1].line - 2)
			{
				players[0].points = players[0].points + 25;
				players[1].health--;
			}
		}
		players[turn].bomb_type = 0;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//win check
	if (players[0].health == 0 || players[0].health < 0)
	{
		players[1].win_check = true;
	}

	else if (players[1].health == 0 || players[1].health < 0)
	{
		players[0].win_check = true;
	}

	//if both players die in teh same turn it is a draw
	else if (players[0].health <= 0 && players[1].health <= 0)
	{
		players[1].win_check = true;
		players[0].win_check = true;
	}
}

int MainMenu()
{
	curs_set(0);

	int rv = 0;

	stringstream ss;
	int x = 0;

	ss << "    _____      _____  ____ _______       _____  ___________ _______   ____ ___ ";
	move(LINES / 3, 17);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << "   /     \\    /  _  \\ |   |\\      \\     /     \\ \\_   _____/ \\      \\ |    |   \\ ";
	move(LINES / 3 + 1, 17);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << "  /  \\ /  \\  /  /_\\  \\|   |/   |   \\   /  \\ /  \\ |    __)_  /   |   \\|    |   /";
	move(LINES / 3 + 2, 17);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << " /    Y    \\/    |    \\   /    |    \\ /    Y    \\|        \\/    |    \\    |  / ";
	move(LINES / 3 + 3, 17);
	addstr(ss.str().c_str());

	ss = stringstream();

	ss << " \\____|__  /\\____|__  /___\\____|__  / \\____|__  /_______  /\\____|__  /______/  ";
	move(LINES / 3 + 4, 17);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << "         \\/         \\/            \\/          \\/        \\/         \\/          ";
	move(LINES / 3 + 5, 17);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << "(P) Play      (S) Settings      (C) Credits      (L) Log      (Q) Quit";
	move(LINES / 3 + 11, 21);
	addstr(ss.str().c_str());

	char c = getch();
	switch (c)
	{
		//play
	case 'p':
	case 'P':
		rv = 1;
		break;

		//credits
	case 'c':
	case'C':
		rv = 2;
		break;

		//settings
	case 's':
	case 'S':
		rv = 3;
		break;

	case 'l':
	case 'L':
		rv = 5;
		break;

		//quit
	case 'q':
	case 'Q':
		rv = 4;
		break;

		//quit


		//add a case where if none of the options are chosen nothing happens
		noecho();
	}
	return rv;
}

void Credits()
{
	erase();
	refresh();
	noecho();
	bool is_b = false;

	while (is_b == false)
	{
		// (up and down, left and right)
		stringstream ss;
		ss << "Game written and developed by Paul A. Fischer.";
		move(LINES / 2 - 4, 38);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "paulfischerproductions@gmail.com";
		move(LINES / 2 - 1, COLS / 2 - 16);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "pfischer@carthage.edu";
		move(LINES / 2, COLS / 2 - 11);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "Professor Perry Kivolowitz, Computer Science II";
		move(LINES / 2 + 3, 37);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "(B) Back";
		move(LINES - 2, COLS - 10);
		addstr(ss.str().c_str());

		char c = getch();
		switch (c)
		{
			//back
		case 'b':
		case 'B':
			is_b = true;
			break;
		}

		initscr();
		noecho();
		keypad(stdscr, 1);
		refresh();
	}
}

void Settings(Player & players)
{
	erase();
	noecho();
	refresh();

	bool keep_going = true;


	while (keep_going == true)
	{
		erase();
		refresh();
		noecho();
		curs_set(0);

		char square = ' ';

		stringstream ss;
		ss << setw(10) << left << "Player 1";
		move(2, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "Player 2";
		move(2, COLS - 18);
		addstr(ss.str().c_str());

		//vertical
		for (int i = 0; i < LINES / 3 * 2 - 2; i++)
		{
			ss = stringstream();
			ss << "|";
			move(i, COLS / 2);
			addstr(ss.str().c_str());
		}

		//horizontal upper
		for (int i = 0; i < COLS; i++)
		{
			ss = stringstream();
			ss << "-";
			move(3, i);
			addstr(ss.str().c_str());
		}

		//horizontal lower
		for (int i = 0; i < COLS; i++)
		{
			ss = stringstream();
			ss << "-";
			move(LINES / 3 * 2 - 2, i);
			addstr(ss.str().c_str());
		}

		ss = stringstream();
		ss << setw(10) << left << "(1) Player 1 Nickname: " << temp_nickname_one;
		move(7, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(2) Player 2 Nickname: " << temp_nickname_two;
		move(7, COLS / 2 + 9);
		addstr(ss.str().c_str());


		ss = stringstream();
		ss << setw(10) << left << "(3) Player 1 Color";
		move(12, 9);
		addstr(ss.str().c_str());

		if (color_toggle_one > 0)
		{
			ss = stringstream();
			ss << "^";
			move(15, 9 + (4 * color_toggle_one) - 4);
			addstr(ss.str().c_str());
		}


		start_color();
		init_pair(1, COLOR_BLACK, COLOR_RED);
		init_pair(2, COLOR_BLACK, COLOR_GREEN);
		init_pair(3, COLOR_BLACK, COLOR_YELLOW);
		init_pair(4, COLOR_BLACK, COLOR_BLUE);
		init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
		init_pair(6, COLOR_BLACK, COLOR_CYAN);
		init_pair(7, COLOR_BLACK, COLOR_WHITE);

		attron(COLOR_PAIR(1));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, 9);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(2));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, 13);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(3));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, 17);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(4));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, 21);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(5));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, 25);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(6));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, 29);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(7));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, 33);
		addstr(ss.str().c_str());

		attroff(COLOR_PAIR(1));
		attroff(COLOR_PAIR(2));
		attroff(COLOR_PAIR(3));
		attroff(COLOR_PAIR(4));
		attroff(COLOR_PAIR(5));
		attroff(COLOR_PAIR(6));
		attroff(COLOR_PAIR(7));

		ss = stringstream();
		ss << setw(10) << right << "(4) Player 2 Color";
		move(12, COLS / 2 + 9);
		addstr(ss.str().c_str());

		if (color_toggle_two > 0)
		{
			ss = stringstream();
			ss << "^";
			move(15, (COLS / 2) + 9 + (4 * color_toggle_two) - 4);
			addstr(ss.str().c_str());
		}


		init_pair(1, COLOR_BLACK, COLOR_RED);
		init_pair(2, COLOR_BLACK, COLOR_GREEN);
		init_pair(3, COLOR_BLACK, COLOR_YELLOW);
		init_pair(4, COLOR_BLACK, COLOR_BLUE);
		init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
		init_pair(6, COLOR_BLACK, COLOR_CYAN);
		init_pair(7, COLOR_BLACK, COLOR_WHITE);

		attron(COLOR_PAIR(1));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, COLS / 2 + 9);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(2));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, COLS / 2 + 13);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(3));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, COLS / 2 + 17);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(4));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, COLS / 2 + 21);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(5));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, COLS / 2 + 25);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(6));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, COLS / 2 + 29);
		addstr(ss.str().c_str());


		attron(COLOR_PAIR(7));
		ss = stringstream();
		ss << setw(2) << right << square;
		move(14, COLS / 2 + 33);
		addstr(ss.str().c_str());

		attroff(COLOR_PAIR(1));
		attroff(COLOR_PAIR(2));
		attroff(COLOR_PAIR(3));
		attroff(COLOR_PAIR(4));
		attroff(COLOR_PAIR(5));
		attroff(COLOR_PAIR(6));
		attroff(COLOR_PAIR(7));



		if (ground_type == 1)
		{
			ss = stringstream();
			ss << setw(10) << right << "(T) Toggle Terrain Type: \t    <Low>          Medium          High ";
			move(20, 9);
			addstr(ss.str().c_str());
			refresh();
		}
		else if (ground_type == 2)
		{
			;
			ss = stringstream();
			ss << setw(10) << right << "(T) Toggle Terrain Type: \t     Low          <Medium>         High ";
			move(20, 9);
			addstr(ss.str().c_str());
			refresh();
		}
		else if (ground_type == 3)
		{
			ss = stringstream();
			ss << setw(10) << right << "(T) Toggle Terrain Type: \t     Low           Medium         <High> ";
			move(20, 9);
			addstr(ss.str().c_str());
			refresh();
		}



		if (wind_change == true)
		{
			ss = stringstream();
			ss << setw(10) << right << "(W) Toggle Wind: \t\t    <True>         False ";
			move(22, 9);
			addstr(ss.str().c_str());
		}
		else if (wind_change == false)
		{
			ss = stringstream();
			ss << setw(10) << right << "(W) Toggle Wind: \t\t     True         <False>";
			move(22, 9);
			addstr(ss.str().c_str());
		}


		ss = stringstream();
		ss << setw(10) << right << "(H) Starting Health \t<" << health_toggle << ">";
		move(24, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(P) Starting Petrol \t<" << gas_toggle << ">";
		move(26, 9);
		addstr(ss.str().c_str());

		if (butter == true)
		{
			ss = stringstream();
			ss << setw(10) << right << "(?) Buttered Toast? \t\t    <Yes>         No ";
			move(28, 9);
			addstr(ss.str().c_str());
		}
		else if (butter == false)
		{
			ss = stringstream();
			ss << setw(10) << right << "(?) Buttered Toast? \t\t     Yes         <No> ";
			move(28, 9);
			addstr(ss.str().c_str());
		}

		ss = stringstream();
		ss << "(B) Back";
		move(LINES - 2, COLS - 10);
		addstr(ss.str().c_str());


		char c = getch();
		switch (c)
		{
			//back
		case 'b':
		case 'B':
			keep_going = false;
			break;


		case '?':
		case '/':
		{
			butter = !butter;
			ft = true;
			break;
		}


		case 'p':
		case 'P':
			if (gas_toggle < 15)
			{
				gas_toggle++;
				break;
			}
			else if (gas_toggle == 15)
			{
				gas_toggle = 0;
				break;
			}

			//health
		case 'h':
		case 'H':
			if (health_toggle < 5)
			{
				health_toggle++;
				break;
			}
			else if (health_toggle == 5)
			{
				health_toggle = 1;
				break;
			}
			

			//wind toggle
		case 'w':
		case 'W':
		{
			if (wind_change == true)
			{
				wind_change = false;
				break;
			}
			else if (wind_change == false)
			{
				wind_change = true;
				break;
			}
		}

		//terrain toggle
		case 't':
		case 'T':
		{
			if (ground_type < 3)
			{
				ground_type++;
				break;
			}
			else if (ground_type == 3)
			{
				ground_type = 1;
				break;
			}
		}

		//p1 nickname
		case '1':
		{
			if (nickname_check_one == true)
			{
				stringstream ss;
				ss << "                    ";
				move(7, 32);
				addstr(ss.str().c_str());
				refresh();

			}
			curs_set(1);
			echo();
			move(7, 32);
			getstr(temp_nickname_one);
			refresh();
			nickname_check_one = true;
			break;
		}


		//p2 nickname
		case '2':
		{
			if (nickname_check_two == true)
			{
				stringstream ss;
				ss << "                    ";
				move(7, COLS / 2 + 32);
				addstr(ss.str().c_str());
				refresh();

			}
			refresh();
			curs_set(1);
			echo();
			move(7, COLS / 2 + 32);
			getstr(temp_nickname_two);
			refresh();
			nickname_check_two = true;
			break;
		}

		//p1 color
		case '3':
		{
			if (color_toggle_one < 7)
			{
				color_toggle_one++;
				break;
			}
			else if (color_toggle_one == 7)
			{
				color_toggle_one = 1;
				break;
			}
		}

		//p2 color
		case '4':
		{
			if (color_toggle_two < 7)
			{
				color_toggle_two++;
				break;
			}
			else if (color_toggle_two == 7)
			{
				color_toggle_two = 1;
				break;
			}
		}

		}
	}
}

void Pointshop(Ground & g, Player * players, int turn)
{
	bool ps = true;

	while (ps == true)
	{
		erase();
		DrawScreen(g, players, turn);
		refresh();
		noecho();

		if (butter == false)
		{
			stringstream ss;

			for (unsigned int i = 0; i < 28; i++)
			{
				mvaddch(2, COLS / 2 - 16 + i, ACS_HLINE);
				mvaddch(16, COLS / 2 - 16 + i, ACS_HLINE);
			}

			for (unsigned int i = 0; i < 14; i++)
			{
				mvaddch(2 + i, COLS / 2 - 16, ACS_VLINE);
				mvaddch(2 + i, COLS / 2 + 12, ACS_VLINE);
			}

			//this means move to this location (row, col, ch) and add the char
			//upper left
			mvaddch(2, COLS / 2 - 16, ACS_ULCORNER);
			//upper right
			mvaddch(2, COLS / 2 + 12, ACS_URCORNER);
			//lower left
			mvaddch(16, COLS / 2 - 16, ACS_LLCORNER);
			//lower right
			mvaddch(16, COLS / 2 + 12, ACS_LRCORNER);
			refresh();



			ss = stringstream();
			ss << "PointShop\t\t  ";
			move(4, COLS / 2 - 14);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "(L) Large Bomb [75c]";
			move(6, COLS / 2 - 14);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "(S) Strong Bomb [75c]";
			move(8, COLS / 2 - 14);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "(H) Health Pack [50c]";
			move(10, COLS / 2 - 14);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "(P) Petrol Jerrycan [25c]";
			move(12, COLS / 2 - 14);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "                          ";
			move(13, COLS / 2 - 14);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "                          ";
			move(14, COLS / 2 - 14);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "                   ";
			move(15, COLS / 2 - 14);
			addstr(ss.str().c_str());


			ss = stringstream();
			ss << "(B) Back";
			move(15, COLS / 2 + 3);
			addstr(ss.str().c_str());
			refresh();
		}

		else if (butter == true)
		{
			stringstream ss;

			for (unsigned int i = 0; i < 28; i++)
			{
				mvaddch(2, COLS / 2 - 21 + i, ACS_HLINE);
				mvaddch(16, COLS / 2 - 21 + i, ACS_HLINE);
			}

			for (unsigned int i = 0; i < 14; i++)
			{
				mvaddch(2 + i, COLS / 2 - 21, ACS_VLINE);
				mvaddch(2 + i, COLS / 2 + 7, ACS_VLINE);
			}

			//this means move to this location (row, col, ch) and add the char
			//upper left
			mvaddch(2, COLS / 2 - 21, ACS_ULCORNER);
			//upper right
			mvaddch(2, COLS / 2 + 7, ACS_URCORNER);
			//lower left
			mvaddch(16, COLS / 2 - 21, ACS_LLCORNER);
			//lower right
			mvaddch(16, COLS / 2 + 7, ACS_LRCORNER);
			refresh();



			ss = stringstream();
			ss << "PointShop\t\t  ";
			move(4, COLS / 2 - 19);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "(L) Large Bomb [75c]";
			move(6, COLS / 2 - 19);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "(S) Strong Bomb [75c]";
			move(8, COLS / 2 - 19);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "(H) Health Pack [50c]";
			move(10, COLS / 2 - 19);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "(P) Petrol Jerrycan [25c]";
			move(12, COLS / 2 - 19);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "                          ";
			move(13, COLS / 2 - 19);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "                          ";
			move(14, COLS / 2 - 19);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << "                   ";
			move(15, COLS / 2 - 19);
			addstr(ss.str().c_str());


			ss = stringstream();
			ss << "(B) Back";
			move(15, COLS / 2 - 2);
			addstr(ss.str().c_str());
			refresh();


			//Buttered Toast (24 lines) should go from 12-36    width
			//////////////////////////////////////////////////////////////////////////////////////////////////////
			int x = 2;

			ss = stringstream();
			ss << setw(46) << left << "          o                o";
			move(12 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "        __|________________|__";
			move(13 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "       /                      \\";
			move(14 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "      |     __          __     |";
			move(15 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "     (|    {_;}        {;_}    |)";
			move(16 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "      |                        |";
			move(17 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "      |                        |";
			move(18 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "       \\                      /";
			move(19 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "        \\_____[~~~~~~~~]_____/";
			move(20 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "  __   ___________||___________   __   \\ | / ";
			move(21 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << " {  }=|    ________________    |={  }  -[=]_/";
			move(22 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "  ||  |   /      ooo   ooo \\   |   \\\\   //";
			move(23 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "  ||  |  |       ooooooooo  |  |    \\\\_//";
			move(24 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << " (  ) |  |         ooooo    |  |     \\_/";
			move(25 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "  ||  |  |___________o______|  |";
			move(26 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << " [=]\\ |        ___  ___        |";
			move(27 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << " /|\\   \\_______/  \\__/_ \\_____/";
			move(28 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "            \\\\ _\\_ _ _ /_/";
			move(29 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "                \\_\\_/_/";
			move(30 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "                   ##";
			move(31 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "                   ## ";
			move(32 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "         #        # ##       # #  ";
			move(33 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "       # # #  # # ##  # #  # # ";
			move(34 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "        # #  # # ## # ## # # #  #";
			move(35 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());

			ss = stringstream();
			ss << setw(46) << left << "       # ##  ##  ##  # ## #  ##  #";
			move(36 + x, COLS / 2 + 13);
			addstr(ss.str().c_str());


			if (ft == true)
			{
				ss = stringstream();
				ss << setw(50) << left << "Hey! I'm Buttered Toast, but you can call me . . .";
				move(12, COLS / 2 + 9);
				addstr(ss.str().c_str());
				refresh();
				Sleep(2750);
				refresh();
			
				ss = stringstream();
				ss << setw(50) << left << "         Butters. Welcome to the shop!";
				move(12, COLS / 2 + 9);
				addstr(ss.str().c_str());
				refresh();
				Sleep(2500);
				refresh();
				ft = false;

				ss = stringstream();
				ss << setw(49) << left << "";
				move(12, COLS / 2 + 10);
				addstr(ss.str().c_str());
				refresh();
			}

			refresh();
			//////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		stringstream ss;
		int r = 4;
	

		int c = getch();
		switch (c)
		{

		case 'b':
		case 'B':
		{
			if (butter == true)
			{
				r = rand() % 3;
			}

			if (r == 0)
			{
				ss = stringstream();
				ss << "Leaving Already?";
				move(12, COLS / 2 + 24);
				addstr(ss.str().c_str());
				refresh();
				Sleep(2000);
				ps = false;
				break;
			}
			else if (r == 1)
			{
				ss = stringstream();
				ss << "Is it something I said?";
				move(12, COLS / 2 + 21);
				addstr(ss.str().c_str());
				refresh();
				Sleep(2000);
				ps = false;
				break;
			}

			else if (r == 2)
			{
				ss = stringstream();
				ss << "Good luck out there!";
				move(12, COLS / 2 + 23);
				addstr(ss.str().c_str());
				refresh();
				Sleep(2000);
				ps = false;
				break;
			}
			else
			{
				ps = false;
				break;
			}
		}

		//Large Bomb - make bomb radius check bigger 75
		case 'l':
		case 'L':
		{
			char credit = 128;


			if (players[turn].bomb_type != 0)
			{
				if (butter == true)
				{
					ss = stringstream();
					ss << "What are you trying to compensate for?";
					move(12, COLS / 2 + 14);
					addstr(ss.str().c_str());

					ss = stringstream();
					ss << "A bomb is already equpped.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				ss = stringstream();
				ss << "A bomb is already equpped.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1500);
				break;
			}

			else if (players[turn].points < 75)
			{
				if (butter == true)
				{
					r = rand() % 3;
					
					if (r == 0)
					{
						ss = stringstream();
						ss << "Does this look like a GoFundMe to you?";
						move(12, COLS / 2 + 14);
						addstr(ss.str().c_str());
					}
					else if (r == 1)
					{
						ss = stringstream();
						ss << "Looks like you are " << 75 - players[turn].points << " credits short.";
						move(12, COLS / 2 + 15);
						addstr(ss.str().c_str());
					}
					else if (r == 2)
					{
						ss = stringstream();
						ss << "Leave your purse in your other tank?";
						move(12, COLS / 2 + 15);
						addstr(ss.str().c_str());
					}
					ss = stringstream();
					ss << "Not enough " << credit << "'s.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				ss = stringstream();
				ss << "Not enough " << credit << "'s.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1200);
				break;
				
			}

			else
			{
				if (butter == true)
				{
					r = rand() % 2;

					if (r == 0)
					{
						ss = stringstream();
						ss << "One sissy missle comin' right up!.";
						move(12, COLS / 2 + 16);
						addstr(ss.str().c_str());
					}
					else if (r == 1)
					{
						ss = stringstream();
						ss << "Losing that bad?";
						move(12, COLS / 2 + 23);
						addstr(ss.str().c_str());
					}
					ss = stringstream();
					ss << "- 75" << credit << "'s.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());

					players[turn].points = players[turn].points - 75;
					players[turn].bomb_type = 2;
					refresh();
					Sleep(2000);
					break;
				}

				ss = stringstream();
				ss << "- 75" << credit << "'s.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());

				players[turn].points = players[turn].points - 75;
				players[turn].bomb_type = 2;
				refresh();
				Sleep(1200);
				break;
			}
		}


		//Strong Bomb - make bomb damage more 75
		case 's':
		case 'S':
		{
			char credit = 128;

			if (players[turn].bomb_type != 0)
			{
				if (butter == true)
				{
					ss = stringstream();
					ss << "What are you trying to compensate for?";
					move(12, COLS / 2 + 14);
					addstr(ss.str().c_str());

					ss = stringstream();
					ss << "A bomb is already equpped.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}
				ss = stringstream();
				ss << "A bomb is already equpped.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1500);
				break;
			}

			else if (players[turn].points < 75)
			{
				if (butter == true)
				{
					r = rand() % 3;

					if (r == 0)
					{
						ss = stringstream();
						ss << "Does this look like a GoFundMe to you?";
						move(12, COLS / 2 + 14);
						addstr(ss.str().c_str());
					}
					else if (r == 1)
					{
						ss = stringstream();
						ss << "Looks like you are " << 75 - players[turn].points << " credits short.";
						move(12, COLS / 2 + 15);
						addstr(ss.str().c_str());
					}
					else if (r == 2)
					{
						ss = stringstream();
						ss << "Leave your purse in your other tank?";
						move(12, COLS / 2 + 15);
						addstr(ss.str().c_str());
					}
					ss = stringstream();
					ss << "Not enough " << credit << "'s.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}
				ss = stringstream();
				ss << "Not enough " << credit << "'s.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1200);
				break;
				
			}

			else
			{
				if (butter == true)
				{
					r = rand() % 2;

					if (r == 0)
					{
						ss = stringstream();
						ss << "One sissy missle comin' right up!.";
						move(12, COLS / 2 + 16);
						addstr(ss.str().c_str());
					}
					else if (r == 1)
					{
						ss = stringstream();
						ss << "Losing that bad?";
						move(12, COLS / 2 + 23);
						addstr(ss.str().c_str());
					}
					ss = stringstream();
					ss << "- 75" << credit << "'s.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);

					players[turn].points = players[turn].points - 75;
					players[turn].bomb_type = 1;
					break;
				}

				ss = stringstream();
				ss << "- 75" << credit << "'s.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1200);

				players[turn].points = players[turn].points - 75;
				players[turn].bomb_type = 1;
				break;
			}
		}

		//Health Refill 50
		case 'h':
		case 'H':
		{
			char credit = 128;

			if (players[turn].points < 50)
			{
				if (butter == true)
				{
					r = rand() % 3;

					if (r == 0)
					{
						ss = stringstream();
						ss << "Does this look like a GoFundMe to you?";
						move(12, COLS / 2 + 14);
						addstr(ss.str().c_str());
					}
					else if (r == 1)
					{
						ss = stringstream();
						ss << "Looks like you are " << 75 - players[turn].points << " credits short.";
						move(12, COLS / 2 + 15);
						addstr(ss.str().c_str());
					}
					else if (r == 2)
					{
						ss = stringstream();
						ss << "Leave your purse in your other tank?";
						move(12, COLS / 2 + 15);
						addstr(ss.str().c_str());
					}
					ss = stringstream();
					ss << "Not enough " << credit << "'s.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				ss = stringstream();
				ss << "Not enough " << credit << "'s.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1200);
				break;
			}

			else if (players[turn].health == 5)
			{
				if (butter == true)
				{
					ss = stringstream();
					ss << "What are you trying to compensate for?";
					move(12, COLS / 2 + 14);
					addstr(ss.str().c_str());

					ss = stringstream();
					ss << "At max health.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				ss = stringstream();
				ss << "At max health.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1500);
				break;
			}

			else
			{

				if (butter == true)
				{
					r = rand() % 3;

					if (r == 0)
					{
						ss = stringstream();
						ss << "Here's a med kit, patch yourself up soldier!";
						move(12, COLS / 2 + 12);
						addstr(ss.str().c_str());
					}
					else if (r == 1)
					{
						ss = stringstream();
						ss << "Need a Band-Aid?";
						move(12, COLS / 2 + 24);
						addstr(ss.str().c_str());
					}
					else if (r == 2)
					{
						ss = stringstream();
						ss << "You know, I got over a 30% on the MCAT.";
						move(12, COLS / 2 + 15);
						addstr(ss.str().c_str());
					}
					ss = stringstream();
					ss << "- 50" << credit << "'s.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);

					players[turn].points = players[turn].points - 50;
					players[turn].health++;
					erase();
					DrawScreen(g, players, turn);
					refresh();
					break;
				}

				ss = stringstream();
				ss << "- 50" << credit << "'s.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1200);

				players[turn].points = players[turn].points - 50;
				players[turn].health++;
				erase();
				DrawScreen(g, players, turn);
				refresh();
				break;
			}

		}

		//Gasoline refill 25
		case 'p':
		case 'P':
		{
			char credit = 128;

			if (players[turn].gas == gas_toggle)
			{
				if (butter == true)
				{
					ss = stringstream();
					ss << "What are you trying to compensate for?";
					move(12, COLS / 2 + 14);
					addstr(ss.str().c_str());

					ss = stringstream();
					ss << "Petrol Full.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				ss = stringstream();
				ss << "Petrol Full.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1500);
				break;
			}

			else if (players[turn].points < 25)
			{
				if (butter == true)
				{
					r = rand() % 3;

					if (r == 0)
					{
						ss = stringstream();
						ss << "Does this look like a GoFundMe to you?";
						move(12, COLS / 2 + 14);
						addstr(ss.str().c_str());
					}
					else if (r == 1)
					{
						ss = stringstream();
						ss << "Looks like you are " << 75 - players[turn].points << " credits short.";
						move(12, COLS / 2 + 15);
						addstr(ss.str().c_str());
					}
					else if (r == 2)
					{
						ss = stringstream();
						ss << "Leave your purse in your other tank?";
						move(12, COLS / 2 + 15);
						addstr(ss.str().c_str());
					}
					ss = stringstream();
					ss << "Not enough " << credit << "'s.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				ss = stringstream();
				ss << "Not enough " << credit << "'s.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1200);
				break;
			}

			else
			{
				if (butter == true)
				{
					ss = stringstream();
					ss << "Don't drink and drive!";
					move(12, COLS / 2 + 21);
					addstr(ss.str().c_str());

					players[turn].points = players[turn].points - 25;
					players[turn].gas = gas_toggle;

					ss = stringstream();
					ss << "- 25" << credit << "'s.";
					move(14, COLS / 2 - 19);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);

					erase();
					DrawScreen(g, players, turn);
					refresh();
					break;
				}
				players[turn].points = players[turn].points - 25;
				players[turn].gas = gas_toggle;

				ss = stringstream();
				ss << "- 25" << credit << "'s.";
				move(14, COLS / 2 - 14);
				addstr(ss.str().c_str());
				refresh();
				Sleep(1200);

				erase();
				DrawScreen(g, players, turn);
				refresh();
				break;
				
			}

		}
		erase();
		refresh();
		DrawScreen(g, players, turn);
		//end of switch loop
		}
		//end of while loop
	}
}


//http://stackoverflow.com/questions/18028808/blinking-underscore-with-console
//this function hides the cursor in c++, found at website above
//if not okay to use, please disregard, only hides cursor on the gameover/c++ function
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void Log()
{
	erase();
	refresh();
	noecho();
	refresh();




	bool is_l = true;

	while (is_l == true)
	{

		stringstream ss;
		ss << "https://github.com/pafischer1995/Tanks-Curses";
		move(LINES / 2 + 2, COLS / 2 - 22);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "https://github.com/pafischer1995/a/blob/master/README.md";
		move(LINES / 2, COLS / 2 - 22);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "Tanks v.2.4.1";
		move(LINES / 2 - 3, COLS / 2 - 7);
		addstr(ss.str().c_str());


		ss = stringstream();
		ss << "(B) Back";
		move(LINES - 2, COLS - 10);
		addstr(ss.str().c_str());
		refresh();
		char c = getch();
		switch (c)
		{
			//back
		case 'b':
		case 'B':
			is_l = false;
			break;
		}

		initscr();
		noecho();
		keypad(stdscr, 1);
		refresh();

	}
}

void GameOver(string w)
{

	//this function hides the cursor, found in void function above gameover
	ShowConsoleCursor(false);

	//http://stackoverflow.com/questions/6899025/hide-user-input-on-password-prompt
	//this hides cin input
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));


	string whitespace;
	whitespace = "\t\t\t\t\t";


	std::cout << whitespace << "  ________    _____      _____  ___________ " << endl;
	std::cout << whitespace << " /  _____/   /  _  \\    /     \\ \\_   _____/ " << endl;
	std::cout << whitespace << "/   \\  ___  /  /_\\  \\  /  \\ /  \\ |    __)_  " << endl;
	std::cout << whitespace << "\\    \\_\\  \\/    |    \\/    Y    \\|        \\ " << endl;
	std::cout << whitespace << " \\______  /\\____|__  /\\____|__  /_______  / " << endl;
	std::cout << whitespace << "        \\/         \\/         \\/        \\/  " << endl;
	std::cout << whitespace << "____________   _________________________    " << endl;
	std::cout << whitespace << "\\_____  \\   \\ /   /\\_   _____/\\______   \\   " << endl;
	std::cout << whitespace << " /   |   \\   Y   /  |    __)_  |       _/   " << endl;
	std::cout << whitespace << "/    |    \\     /   |        \\ |    |   \\   " << endl;
	std::cout << whitespace << "\\_______  /\\___/   /_______  / |____|_  /   " << endl;
	std::cout << whitespace << "        \\/                 \\/         \\/    " << endl;
	std::cout << "\n" << endl;
	if (w == "Draw")
	{
		std::cout << whitespace << "\t\t  " << w << endl;
	}
	else
	{
		std::cout << setw(53) << " " << w << " wins!" << endl;
	}
	std::cout << "\n" << endl;
	std::cout << whitespace << "	     Play Again? Y/N \n\n" << endl;
	std::cout << "\n\n\n\n" << endl;
}


int main(int argc, char * argv[])
{
	
	//setlocale(LC_ALL, "");

	//was trying to use UTF-8, UTF-16, or Unicode. Tried to use the line above, as well as the _setmode function
	//when on a page that was using c++ it worked, else it didn't
	//couldn't implement because if I broke to use c++ to use icons, if I went to do initscr() it would just clear all the stuff I had done.

	//this loops from the game over menu back to start
	while (true)
	{

		bool quit = true;

		//this loops back to the starting menu
		while (true)
		{
			quit = true;

			erase();
			initscr();
			noecho();
			keypad(stdscr, 1);
			refresh();

			int x = 0;
			x = MainMenu();

			//quit

			if (x == 5)
			{
				initscr();
				noecho();
				keypad(stdscr, 1);
				refresh();
				Log();
			}

			if (x == 4)
			{
				quit = false;
				break;
			}

			//credits
			else if (x == 2)
			{
				initscr();
				noecho();
				keypad(stdscr, 1);
				refresh();
				Credits();

			}

			//settings
			else if (x == 3)
			{
				initscr();
				noecho();
				keypad(stdscr, 1);
				refresh();
				Settings(Player());
			}


			//play
			else if (x == 1)
			{
				break;
			}
		}

		if (quit == false)
		{
			break;
		}


		//this while loop starts the game over
		while (true)
		{
			bool keep_going = true;
			srand((unsigned int)time(nullptr));

			int turn = 0;

			string w = "Draw";
			Ground g;
			Player players[2];

			if (nickname_check_one == true)
			{
				players[0].nick = true;
			}

			if (nickname_check_two == true)
			{
				players[1].nick = true;
			}


			players[0].health = players[0].health + health_toggle;
			players[1].health = players[1].health + health_toggle;

			players[0].gas = players[0].gas + gas_toggle;
			players[1].gas = players[1].gas + gas_toggle;


			players[0].color = color_toggle_one;
			players[1].color = color_toggle_two;

			players[0].nickname = temp_nickname_one;
			players[1].nickname = temp_nickname_two;

			players[0].points = p1s;
			players[1].points = p2s;

			Wind();

			g.InitializeGround();
			
			//players.initialize spawns the tanks location
			players[0].Initialize(rand() % (cols / 4), LEFT);
			players[1].Initialize(rand() % (cols / 4) + 3 * cols / 4 - 2, RIGHT);

			initscr();
			noecho();
			resize_term(lines, cols);
			keypad(stdscr, 1);
			curs_set(0);


			//drawscreen does draw(g) and draw(players) as well as drawsettings(players)
			//draw creates the tank
			//drawsettings creates the words, gas, and health

			DrawScreen(g, players, turn);
	

			//this loop is a players turn
			while (keep_going)
			{
				erase();
				refresh();

				DrawScreen(g, players, turn);

				int x = turn;
				int winner_check = 0;

				//keep bullet in loop so that it doesn't hurt player 2 rounds in a row
				int  bullet_impact_horizontal = 0;
				int  bullet_impact_vertical = 0;

				if (show == true)
				{
					stringstream ss;
					ss << players[turn].col;
					move(LINES - 1, COLS / 2 - 51);
					addstr(ss.str().c_str());

					ss = stringstream();
					ss << players[turn].line;
					move(LINES - 1, COLS / 2 - 41);
					addstr(ss.str().c_str());
				}
				refresh();

				bool show_char = false;
				int c = getch();
				switch (c)
				{
				case 27:
					keep_going = false;
					break;


				case KEY_LEFT:		//left arrow
					if (players[turn].s == RIGHT)
					{
						players[turn].PowerUp();
						break;
					}
					else
						players[turn].PowerDown();
					break;


				case KEY_RIGHT:		//right arrow
					if (players[turn].s == RIGHT)
					{
						players[turn].PowerDown();
						break;
					}
					else
						players[turn].PowerUp();
					break;

				case KEY_UP:		//up arrow
					players[turn].AngleUp();
					break;

				case KEY_DOWN:		//down arrow
					players[turn].AngleDown();
					break;

				case 10:
				case KEY_ENTER:
				case PADENTER:
				case ' ':
				{
					Shoot(g, players, turn, bullet_impact_horizontal, bullet_impact_vertical);
					//this makes it so each player regenerates one gas a turn unless at max tank size
					if (players[turn].gas < gas_toggle)
					{
						players[turn].gas = players[turn].gas + 1;
						turn = 1 - turn;
						Wind();
						break;
					}
					else
						turn = 1 - turn;
					Wind();
					break;
				}

				//move left
				case 'a':
				case 'A':
					if (players[turn].gas > 0 && players[turn].col > 0)
					{
						//if the ground to the left is more than 1 higher up you can't move past
						if (g.ground.at(players[turn].col) - g.ground.at(players[turn].col - 1) >= 2)
						{
							continue;
						}
						//tank can't move past the other
						else if (players[0].col + 1 == players[1].col)
						{
							break;
						}

							//move, subtract gas
							players[turn].col--;
							players[turn].gas--;
							break;
						
						
					}
					else
						continue;

					//move right
				case 'd':
				case 'D':
					if (players[turn].gas > 0 && players[turn].col < 117)
					{
						//if the ground to the right is more than 1 higher up you can't move past
						if (g.ground.at(players[turn].col) - g.ground.at(players[turn].col + 1) >= 2)
						{
							break;
						}
						//tank can't move past the other
						else if (players[1].col - 1 == players[0].col)
						{
							break;
						}

						else
						{
							//move, subtract gas
							players[turn].col++;
							players[turn].gas--;
							break;
						}
					
					}
					else
						continue;

				case 'p':
				case 'P':
				{
					Pointshop(g, players, turn);
					break;
				}


				case KEY_DL:
				case KEY_DC:
				case KEY_BACKSPACE:
				{
					if (players[turn].gas + 1 == gas_toggle)
					{
						players[turn].gas = players[turn].gas + 1;
						turn = 1 - turn;
						Wind();
						break;
					}
					else if (players[turn].gas < gas_toggle)
					{
						players[turn].gas = players[turn].gas + 2;
						turn = 1 - turn;
						Wind();
						break;
					}
					else
					{
						turn = 1 - turn;
						Wind();
						break;
					}
				}

				//for presentation purposes, M = MONEYMONEYMONEYMONEY or MAKEITRAINNN
				case 'm':
				case 'M':
				{
					players[turn].points = players[turn].points + 50;
					refresh();
					break;
				}

				case 's':
				case 'S':
					show = !show;
					erase();
					DrawScreen(g, players, turn);
					break;

				case 'w':
				case 'W':
					Wind();
					break;

				case 'r':
				case 'R':
					land = !land;
					DrawScreen(g, players, turn);
					break;

				case 'q':
				case 'Q':
						g.InitializeGround();
					keep_going = false;
					break;

				default:
					//show_char = false;
					break;
				}

				

				if (c != 'q' || c != 'Q')
				{
					DrawScreen(g, players, turn);

					if (show_char)
					{
						move(0, 1);
						stringstream ss;
						ss << setw(4) << c << " ";
						addstr(ss.str().c_str());
						refresh();
					}


					if (players[0].win_check == true)
					{
						if (nickname_check_one == true)
						{
							w = players[0].nickname;
						}
						else
						{
							w = "Player 1";
						}

						keep_going = false;
						players[0].points = players[0].points + 50;
					}

					else if (players[1].win_check == true)
					{
						if (nickname_check_two == true)
						{
							w = players[1].nickname;
						}
						else
						{
							w = "Player 2";
						}

						keep_going = false;
						players[1].points = players[1].points + 50;

					}

					//if both die in same turn
					else if (players[1].win_check == true && players[2].win_check == true)
					{

						w = "Draw";

						keep_going = false;
						players[1].points + 50;
						players[0].points + 50;

					}
				}
				
				p1s = players[0].points;
				p2s = players[1].points;
			}

			erase();
			endwin();

			GameOver(w);



			char input = ' ';

			while (input == ' ')
			{
				input = _getch();


				//if you hit Y it starts over
				if ((input == 'y') || (input == 'Y') || (input == 'n') || (input == 'N'))
				{
					break;
				}

				else
				{
					input = ' ';
				}
			}

			if (input == 'n' || input == 'N')
			{
				break;
			}
			else
			{
				continue;
			}


#if defined(WIN32)
			system("pause");
#endif
			return 0;
		}
	}





	//Bugs

	//try to make health hearts
	//if you can get unicode or UTF-8 to work change ground as well
	//because I moved ground over one to the right to fix the smoothing of the floor the ground.at(1) doesn't change. oopsies.
}
