﻿//Paul A. Fischer, Don Jervis
//Computer Science II, Perry Kivolowitz
//Final Project, 4/27/17-5/9/17
//Tanks


#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cmath>
#include <Windows.h>
#include <locale.h>


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
//projects the path of the bullet as *
void Shoot(Ground & g, Player * players, int turn)
{
	double angle = players[turn].angle / 180.0 * PI;
	double y_component = sin(angle) * players[turn].power * 0.2;
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
		if (pNx < 1 || pNx >= cols - 2)
			break;
		if (pNy < 1) {
			Sleep(50);
			continue;
		}
		//	if (pNy >= lines - 2)
		//		break;
		if (pNy > g.ground.at((int)pNx))
			break;

		move((int)pNy - 1, (int)pNx + 1);
		addch(ACS_BULLET);
		refresh();
		Sleep(50);
	}
}


int MainMenu()
{

	int rv = 0;

	stringstream ss;
	int x = 0;

	ss << "    _____      _____  ____ _______       _____  ___________ _______   ____ ___ ";
	move(LINES /3,17);
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

	//this one puts the blinking cursor at the bottom left corner on main menu
	ss = stringstream();
	ss << " ";
	move(LINES - 1, 0);
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

	stringstream ss;
	ss << "Game written and developed by Paul Fischer.";
	move(LINES / 2, 38);
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
		break;
	}

		initscr();
		noecho();
		keypad(stdscr, 1);
		refresh();

}

void Settings()
{
	erase();
	refresh();
	noecho();

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
		move(LINES / 3 * 2 -2, i);
		addstr(ss.str().c_str());
	}

	ss = stringstream();
	ss << setw(10) << left << "Player 1 Nickname: ";
	move(8,9);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << setw(10) << right << "Player 2 Nickname: ";
	move(8, COLS/2 + 9);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << setw(10) << left << "Player 1 Color: ";
	move(13, 9);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << setw(10) << right << "Player 2 Color: ";
	move(13, COLS / 2 + 9);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << setw(10) << right << "Terrain Type: \t\t (H) High \t\t (M) Medium \t\t (L) Low ";
	move(21, 9);
	addstr(ss.str().c_str());

	ss = stringstream();
	ss << setw(10) << right << "Wind: \t\t\t (H) High \t\t (M) Medium \t\t (L) Low ";
	move(25, 9);
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
		break;
	}

	initscr();
	noecho();
	keypad(stdscr, 1);
	refresh();

}

void GameOver()
{
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

	cout << whitespace << "   Player INSERT WINNING PLAYER wins!" << endl;
	cout << "\n" << endl;
	cout << whitespace << "	     Play Again? Y/N \n\n" << endl;
	cout << "\n\n\n\n" << endl;
}


int main(int argc, char * argv[])
{


	while (true)
	{
		bool quit = true;

		while (true)
		{
			erase();
			initscr();
			noecho();
			keypad(stdscr, 1);
			refresh();

			int x = 0;;
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
				Settings();
			}

			//player
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

			srand((unsigned int)time(nullptr));

			int turn = 0;
			bool keep_going = true;

			Ground g;
			Player players[2];

			initscr();
			noecho();
			resize_term(lines, cols);
			keypad(stdscr, 1);
			curs_set(0);

			g.InitializeGround();
			players[0].Initialize(rand() % (cols / 4), LEFT);
			players[1].Initialize(rand() % (cols / 4) + 3 * cols / 4 - 2, RIGHT);

			DrawScreen(g, players, turn);



			while (keep_going)
			{

				int x = turn;

				bool show_char = false;
				int c = getch();
				switch (c)
				{
				case 27:
					keep_going = false;
					break;

					//			L							2
					//	     C-    C					4		5
					//			L-							3


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
					Shoot(g, players, turn);
					//this makes it so each player regenerates one gas a turn unless at max tank size
					if (players[turn].gas < 5)
					{
						players[turn].gas++;
					}

					turn = 1 - turn;
					break;


					//move left
				case 'a':
				case 'A':
					players[turn].AngleUp();
					if (players[turn].gas > 0)
					{
						//move, subtract gas
						players[turn].gas--;
						break;
					}
					else
						continue;

					//move right
				case 'd':
				case 'D':
					players[turn].AngleDown();
					if (players[turn].gas > 0)
					{
						//move, subtract gas
						players[turn].gas--;
						break;
					}
					else
						continue;


				case 'q':
				case 'Q':
					keep_going = false;
					break;

				default:
					show_char = true;
					break;
				}


				DrawScreen(g, players, turn);
				if (show_char) {
					move(0, 1);
					stringstream ss;
					ss << setw(4) << c << " ";
					addstr(ss.str().c_str());
					refresh();
				}

			}

			erase();
			refresh();
			echo();
			endwin();	//terminates your game, you can use cout after this.


			GameOver();


			char input;
			std::cin >> input;

			//if you hit Y it starts over
			if ((input == 'y') || (input == 'Y'))
			{
				continue;
			}
			//if you hit anything else it goes back to menu page
			else
			{
				break;
			}




#if defined(WIN32)
			system("pause");
#endif
			return 0;
		}
	}
}


//extra credit Ideas

//- wind
//- ground desctruction
//- look to see if you can change the color of a tank(s)
//player names
//- see if you can get a visual for shooting
//- move/gas
//-different terrains in the settings
//bombs and armour
//end of bullet disappear after it's 10 long

/*
ASCII code 176 = ░ ( Graphic character, low density dotted )
ASCII code 177 = ▒ ( Graphic character, medium density dotted )
ASCII code 178 = ▓ ( Graphic character, high density dotted )

//how player will lose health
//-so low power that it falls in radius and kills him
//-wind pushes it back and it hits him
//-other player hits him
//-shoots straight up (no wind) and it comes down and hits him.
//these all have to do with the same thing, if the bomb is within radius of EITHER tank, the tank takes damage
*/