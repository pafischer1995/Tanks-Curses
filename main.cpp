//Paul A. Fischer, Don Jervis
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
		addch('*');
		refresh();
		Sleep(50);
	}
}



int main(int argc, char * argv[])
{
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
			case '<':
				players[turn].PowerDown();
				break;

			case KEY_RIGHT:		//right arrow
			case '>':
				players[turn].PowerUp();
				break;

			case KEY_UP:		//up arrow
			case 'u':
				players[turn].AngleUp();
				break;

			case KEY_DOWN:		//down arrow
			case 'd':
				players[turn].AngleDown();
				break;

			case 10:
			case KEY_ENTER:
			case PADENTER:
				Shoot(g, players, turn);
				turn = 1 - turn;
				break;

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
		/*
		erase();
		addstr("Hit any key to exit");
		refresh();
		getch();
		echo();
		*/
		endwin();	//terminates your game, you can use cout after this.

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
		cout << "\n\n" << endl;
		cout << whitespace << " Created by: Paul Fischer & Don Jervis" << endl;
		cout << whitespace << "   Player INSERT WINNING PLAYER wins!" << endl;
		cout << whitespace << "	   Play Again? Y/N \n\n" << endl;

		char input;
		cin >> input;

		if ((input == 'y') || (input == 'Y'))
		{
			continue;
		}
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


//extra credit Ideas
//- choose amount of players
//- wind
//- smooth ground
//- ground desctruction
//- look to see if you can change the color of a tank(s)
//- see if you can get a visual for shooting
//- move/gas
//- a visual on number of lives for player
//if ground is gone beneath player but he still has health, he dies

/*
ASCII code 176 = ░ ( Graphic character, low density dotted )
ASCII code 177 = ▒ ( Graphic character, medium density dotted )
ASCII code 178 = ▓ ( Graphic character, high density dotted )
empty heart &#9825;
full heart  &#9829;
*/

/*
Notes:
linker allows us to attach different source files together. for this project we have three.
-main
-ground
-player
*/

//how player will lose health
//-so low power that it falls in radius and kills him
//-wind pushes it back and it hits him
//-other player hits him
//-shoots straight up (no wind) and it comes down and hits him.
