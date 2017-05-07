//Paul A. Fischer, Don Jervis
//Computer Science II, Perry Kivolowitz
//Final Project, 4/27/17-5/9/17
//Tanks


// (up and down, left and right)
//stringstream cannot use curses/ncurses operations
//use stringstream for c++
//use addch and char* for ncurses

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
int p1s = 250;						//player 1 score storage
int p2s = 0;						//player 2 score storage

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


void Shoot(Ground & g, Player * players, int turn, int bih, int biv)
{


	double angle = players[turn].angle / 180.0 * PI;
	//vertical
	double y_component = sin(angle) * players[turn].power * 0.2;
	//horizontal
	double x_component = cos(angle) * players[turn].power * 0.2;

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
		double di = i / 5.0;

		pNx = (int)(p0x + di * x_component);
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
			if (pNy >= lines - 2)
			break;
		
		//if the bullet is in the ground then break
			if (pNy >= g.ground.at((int)pNx))
			{
				g.ground.at((int)pNx)++;
				g.ground.at((int)pNx - 1)++;
				g.ground.at((int)pNx + 1)++;
				break;
			}

		//this makes the bullet only one
		erase();
		DrawScreen(g, players, turn);
		move((int)pNy - 1, (int)pNx + 1);
		addch(ACS_BULLET);


		refresh();
		Sleep(50);
	}
	
	bih = pNx + 1;
	biv = pNy - 1;

	stringstream ss;
	ss = stringstream();
	ss << "col: " << bih;
	move(1, COLS / 2 - 3);
	addstr(ss.str().c_str());
	refresh();

	ss = stringstream();
	ss << "line: " << biv;
	move(2, COLS / 2 - 3);
	addstr(ss.str().c_str());
	refresh();

	ss = stringstream();
	ss << "#";
	move(biv,bih);
	addstr(ss.str().c_str());
	refresh();

	Sleep(1200);

	/*
	//large bomb (does 2 damage)
	if (players[turn].bomb_type == 1)
	{

	}

	//strong bomb (bigger radius)
	if (players[turn].bomb_type == 2)
	{

	}
	*/

	//if bomb is within 1 column in either direction of player 1 or on the column
	if (bih == players[0].col || bih == players[0].col + 1 || bih == players[0].col - 1)
	{
		if (biv == players[0].line || biv == players[0].line + 1 || biv == players[0].line - 1)
		{
			players[1].points = players[0].points + 25;
			players[0].health--;
		}
	}

	//if bomb is within 1 column in either direction of player 2
	if (bih == players[1].col || bih == players[1].col + 1 || bih == players[1].col - 1)
	{
		if (biv == players[1].line || biv == players[1].line + 1 || biv == players[1].line - 1)
		{
			players[0].points = players[1].points + 25;
			players[1].health--;
		}
	}


	//win check
	if (players[0].health == 0)
	{
		players[1].win_check = true;
	}

	if (players[1].health == 0)
	{
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
	ss << "(P) Play \t\t (S) Settings \t\t (C) Credits \t\t (Q) Quit";
	move(LINES / 3 + 11, 17);
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


		//quit
	case 'q':
	case 'Q':
		rv = 4;
		break;

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

		ss = stringstream();
		ss << setw(10) << right << "(T) Toggle Terrain Type: \t\t High \t\t Medium \t   Low ";
		move(21, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(W) Toggle Wind: \t\t\t High \t\t Medium \t   Low ";
		move(23, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(H) Starting Health \t<" << health_toggle << ">";
		move(25, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(P) Starting Petrol \t<" << gas_toggle << ">";
		move(27, 9);
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
			keep_going = false;
			break;


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
			if (health_toggle == 5)
			{
				health_toggle = 1;
				break;
			}

			//wind toggle
		case 'w':
		case 'W':
			break;

			//terrain toggle
		case 't':
		case 'T':
			break;

			//p1 nickname
		case '1':
		{
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
			ss << "(B) Back";
			move(15, COLS / 2 + 3);
			addstr(ss.str().c_str());

		int c = getch();
		switch (c)
		{

			case 'b':
			case 'B':
			{
				ps = false;
				break;
			}

			//Large Bomb - make bomb radius check bigger 75
			case 'l':
			case 'L':
			{
				char credit = 128;
				
				if (players[turn].bomb_type != 0)
				{
					ss = stringstream();
					ss << "A bomb is already equpped.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				else if (players[turn].points < 75)
				{
					ss = stringstream();
					ss << "Not enough " << credit << "'s.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(1500);
					break;
				}

				else
				{
					ss = stringstream();
					ss << "- 75" << credit << "'s.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());

					players[turn].points = players[turn].points - 75;
					players[turn].bomb_type = 2;
					refresh();
					Sleep(1500);
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
					ss = stringstream();
					ss << "A bomb is already equpped.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				else if (players[turn].points < 75)
				{
					ss = stringstream();
					ss << "Not enough " << credit << "'s.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(1500);
					break;
				}

				else
				{
					ss = stringstream();
					ss << "- 75" << credit << "'s.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(1500);

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
					ss = stringstream();
					ss << "Not enough " << credit << "'s.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(1500);
					break;
				}

				else if (players[turn].health == 5)
				{
					ss = stringstream();
					ss << "At max health.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				else
				{

					ss = stringstream();
					ss << "- 50" << credit << "'s.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(1500);

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
					ss = stringstream();
					ss << "Petrol Full.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(2000);
					break;
				}

				else if (players[turn].points < 25)
				{
					ss = stringstream();
					ss << "Not enough " << credit << "'s.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(1500);
					break;
				}

				else
				{
					players[turn].points = players[turn].points - 25;
					players[turn].gas = gas_toggle;

					ss = stringstream();
					ss << "- 25" << credit << "'s.";
					move(14, COLS / 2 - 14);
					addstr(ss.str().c_str());
					refresh();
					Sleep(1500);

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
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
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


	cout << whitespace << "  ________    _____      _____  ___________ " << endl;
	cout << whitespace << " /  _____/   /  _  \\    /     \\ \\_   _____/ " << endl;
	cout << whitespace << "/   \\  ___  /  /_\\  \\  /  \\ /  \\ |    __)_  " << endl;
	cout << whitespace << "\\    \\_\\  \\/    |    \\/    Y    \\|        \\ " << endl;
	cout << whitespace << " \\______  /\\____|__  /\\____|__  /_______  / " << endl;
	cout << whitespace << "        \\/         \\/         \\/        \\/  " << endl;
	cout << whitespace << "____________   _________________________    " << endl;
	cout << whitespace << "\\_____  \\   \\ /   /\\_   _____/\\______   \\   " << endl;
	cout << whitespace << " /   |   \\   Y   /  |    __)_  |       _/   " << endl;
	cout << whitespace << "/    |    \\     /   |        \\ |    |   \\   " << endl;
	cout << whitespace << "\\_______  /\\___/   /_______  / |____|_  /   " << endl;
	cout << whitespace << "        \\/                 \\/         \\/    " << endl;
	cout << "\n" << endl;
	if (w == "Draw")
	{
		cout << whitespace << "\t\t  " << w << endl;
	}
	else
	{
		cout << setw(53) << " " << w << " wins!" << endl;
	}
	cout << "\n" << endl;
	cout << whitespace << "	     Play Again? Y/N \n\n" << endl;
	cout << "\n\n\n\n" << endl;
}


int main(int argc, char * argv[])
{

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

		bool keep_going = true;
		//this while loop starts the game over
		while (true)
		{
			keep_going = true;
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

			initscr();
			noecho();
			resize_term(lines, cols);
			keypad(stdscr, 1);
			curs_set(0);


			g.InitializeGround();
			//players.initialize spawns the tanks location
			players[0].Initialize(rand() % (cols / 4), LEFT);
			players[1].Initialize(rand() % (cols / 4) + 3 * cols / 4 - 2, RIGHT);


			//drawscreen does draw(g) and draw(players) as well as drawsettings(players)
			//draw creates the tank
			//drawsettings creates the words, gas, and health
			DrawScreen(g, players, turn);

			refresh();

			//this loop is a players turn
			while (keep_going)
			{

				int x = turn;
				int winner_check = 0;


				//keep bullet in loop so that it doesn't hurt player 2 rounds in a row
				int  bullet_impact_horizontal = 0;
				int  bullet_impact_vertical = 0;

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
						break;
					}
					else
						turn = 1 - turn;
						break;
				}

					//move left
				case 'a':
				case 'A':
					if (players[turn].gas > 0)
					{
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
					if (players[turn].gas > 0)
					{
						//move, subtract gas
						players[turn].col++;
						players[turn].gas--;
						break;
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
						break;
					}
					else if (players[turn].gas < gas_toggle)
					{
						players[turn].gas = players[turn].gas + 2;
						turn = 1 - turn;
						break;
					}
					else
					{
						turn = 1 - turn;
						break;
					}
				}
				

				case 'q':
				case 'Q':
					keep_going = false;
					break;

				default:
					show_char = true;
					break;
				}

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

				if (players[1].win_check == true)
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

				p1s = players[0].points;
				p2s = players[1].points;

			}

			erase();
			refresh();
			echo();
			endwin();	//terminates your game, you can use cout after this.

						//play game over screen (not ncurses)
			GameOver(w);





			char input = ' ';

			while ((input != 'y') || (input != 'Y') || (input != 'n') || (input != 'N'))
			{
			input =	_getch();


				//if you hit Y it starts over
				if ((input == 'y') || (input == 'Y') || (input == 'n') || (input == 'N'))
				{
					break;
				}

				else 
				{
					continue;
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
	

	//try to make health hearts
	//make health red

	//Bugs

	//-fix flash that occurs during 'play'
	//-ground doesn't take damage after a certain column
	//-bug where it has to have main menu x2 times to get a fresh map (this is due to the DrawScreen function)
	//-game crashed once when bomb went off screen at the very bottom right corner, didn't hit ground
	//-first ground is still wrong

	//extra credit Ideas

	//if the ground doesn't have an icon make it so you can't move past it
	//- wind
	//-different terrains in the settings (as is is medium. flat is 1, high elevation is 3
	//-point shop
	//	+1 health
	//	petrol canister
	//	big bomb
	//	strong bomb
	//make a ascii robot thing that welcomes player to pointshop, they can say sorry you don't have credits, or maybe a random line about purchasing the thing they bought


	/*
	//fill ground with ASCII Table below, or ACS_CKBOARD
	Possibly populate ground with these (depending on density maybe?)
	ASCII code 176 = ░ ( Graphic character, low density dotted )
	ASCII code 177 = ▒ ( Graphic character, medium density dotted )
	ASCII code 178 = ▓ ( Graphic character, high density dotted )
	*/
}

