//Paul A. Fischer, Don Jervis
//Computer Science II, Perry Kivolowitz
//Final Project, 4/27/17-5/9/17
//Tanks


// (up and down, left and right)
//if you q twice in a row the second time it brings you to the main menu. check

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
int gas_toggle = 0;
int health_toggle = 0;

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
void Shoot(Ground & g, Player * players, int turn, int bih, int biv)
{


	double angle = players[turn].angle / 180.0 * PI;
	//vertical
	double y_component = sin(angle) * players[turn].power * 0.2;
	//horizontal
	double x_component = cos(angle) * players[turn].power * 0.2;

	double pNx;		//position new x and new y?
	double pNy;

	//flips the bullet if it's the player on the other side of the screen
	if (players[turn].s == RIGHT)
		x_component = -x_component;

	double p0x = players[turn].col;
	double p0y = g.ground.at(players[turn].col);
	// higher ground numbers are lower altitudes (0 is first line, etc).

	p0y = lines - p0y;

	int bullet_length = 0;

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

		//this breaks the ground
		if (pNy == g.ground.at((int)pNx))
		{
			g.ground.at((int)pNx) = g.ground.at((int)pNx) + 1;
		//g.ground.at((int)pNx + 1) = g.ground.at((int)pNx + 1) + 1;
		//g.ground.at((int)pNx - 1) = g.ground.at((int)pNx - 1) - 1;
		}
		
//this makes the bullet only one
				erase();
				DrawScreen(g, players, turn);
				bullet_length = 0;
				move((int)pNy - 1, (int)pNx + 1);
				addch(ACS_BULLET);
			
			
			refresh();
			Sleep(50);
	}
						//pNx is left and right(cols), pNy is up and down(rows), 0,0 is top left
	bih = pNx;
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

	Sleep(1200);



	//if bomb is within 1 column in either direction of player 1 or on the column
	if (bih == players[0].col || bih == players[0].col +1 || bih == players[0].col - 1)
	{
		if (biv == players[0].line || biv == players[0].line + 1 || biv == players[0].line - 1)
		{
			players[0].health--;
		}
	}

	//if bomb is within 1 column in either direction of player 2
	if (bih == players[1].col || bih == players[1].col +1 || bih == players[1].col - 1)
	{
		if (biv == players[1].line || biv == players[1].line + 1 || biv == players[1].line - 1)
		{
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
	move(LINES / 2, COLS /2 - 11);
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
		break;
	}

		initscr();
		noecho();
		keypad(stdscr, 1);
		refresh();

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
		ss << setw(10) << left << "(1) Player 1 Nickname: ";
		move(8, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(2) Player 2 Nickname: ";
		move(8, COLS / 2 + 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << left << "(3) Player 1 Color: ";
		move(13, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(4) Player 2 Color: ";
		move(13, COLS / 2 + 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(T) Toggle Terrain Type: \t\t High \t\t Medium \t   Low ";
		move(21, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(W) Toggle Wind: \t\t\t High \t\t Medium \t   Low ";
		move(23, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(H) Starting Health \t<" << Player().health << ">";
		move(25, 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << setw(10) << right << "(P) Starting Petrol \t<" << Player().gas << ">";
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
		if (gas_toggle < 10)
			{
				gas_toggle++;
				break;
			}
		else if (gas_toggle == 10)
		{
			gas_toggle = -5;
			break;
		}
	
			//health
		case 'h':
		case 'H':
			if (health_toggle < 2)
			{
				health_toggle++;
				break;
			}
			if (health_toggle == 2)
			{
				health_toggle = -2;
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
			break;

			//p2 nickname
		case '2':
			break;

			//p1 color
		case '3':
			break;

			//p2 color
		case '4':
			break;
		}
		
	}
}

void GameOver(string w)
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
	if (w == "Draw")
	{
		cout << whitespace << "\t\t  " << w << endl;
	}
	else
	{
		cout << whitespace << "\t     " << w << " wins!" << endl;
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

			srand((unsigned int)time(nullptr));

			int turn = 0;
			bool keep_going = true;
			string w = "Draw";

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
					Shoot(g, players, turn, bullet_impact_horizontal, bullet_impact_vertical);
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
					w = "Player 1";
					keep_going = false;
				}

				if (players[1].win_check == true)
				{
					w = "Player 2";
					keep_going = false;
				}
				

			}

			erase();
			refresh();
			echo();
			endwin();	//terminates your game, you can use cout after this.

			//play game over screen (not ncurses)
			GameOver(w);


			char input = getchar();
			

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

//work on ground contstruction- add the '|' symbol if they aren't on the same line (and don't allow up)
//look into making the ground destruction move by 3, that way it could possibly smooth it down

//extra credit Ideas

//- wind
//- look to see if you can change the color of a tank(s)
//- player names
//- see if you can get a visual for shooting
//-different terrains in the settings
//- bombs and armour


/*
//fill ground with ASCII Table below, or ACS_CKBOARD
Possibly populate ground with these (depending on density maybe?)
ASCII code 176 = ░ ( Graphic character, low density dotted )
ASCII code 177 = ▒ ( Graphic character, medium density dotted )
ASCII code 178 = ▓ ( Graphic character, high density dotted )
*/