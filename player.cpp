#include <sstream>
#include <iomanip>
#include "curses.h"
#include "player.hpp"
#include <locale.h>
#include <clocale>

using namespace std;

extern int lines;
extern int cols;
extern int base_height_divisor;
extern int max_height_divisor;


const int Player::power_increment = 1;
const double Player::angle_increment = 1;

//this is only incremented the first time
Player::Player()
{
	s = LEFT;
	col = 0;
	line = 0;
	angle = 35.0;
	power = 50.0;
	win_check = false;
}


void Player::Initialize(int column, Side side)
{
	col = column;
	s = side;
}

//not moving
void Player::Draw(Ground & g)
{
	mvaddch(g.ground.at(col) - 1, col + 1, ACS_DIAMOND);

	//in case there is ever a situation where land is floating above, or they hit the land below, they don't die for being in same column
	line = g.ground.at(col) - 1;
}



void Player::PowerUp()
{
	power += power_increment;
}


void Player::PowerDown()
{
	power -= power_increment;
	if (power <= 0)
		power = power_increment;
}


void Player::AngleUp()
{
	angle += angle_increment;
	if (angle >= 90.0)
		angle = 90.0;
}

void Player::AngleDown()
{
	angle -= angle_increment;
	if (angle < 0.0)
		angle = 0.0;
}



void Player::DrawSettings(int turn)
{
	bool my_turn = (turn == 0 && s == LEFT) || (turn == 1 && s == RIGHT);

	int starting_column = 2;
	int line = 1;
	char player = (s == LEFT) ? '1' : '2';
	stringstream ss;

	if (s == RIGHT)
		starting_column = cols - 18;

	if (my_turn)
		attron(A_STANDOUT);
	ss << setw(10) << left << "Player: " << player;
	mvaddstr(line++, starting_column, ss.str().c_str());
	if (my_turn)
		attroff(A_STANDOUT);

	ss = stringstream();
	ss << setw(10) << left << "Angle: " << setw(6) << angle;
	mvaddstr(line++, starting_column, ss.str().c_str());

	ss = stringstream();
	ss << setw(10) << left << "Power: " << setw(6) << power;

	mvaddstr(line++, starting_column, ss.str().c_str());

	ss = stringstream();
	for (unsigned int i = 0; i < health; i++)
	{
		ss << "X ";
	}
	mvaddstr(line++, starting_column, ss.str().c_str());

	ss = stringstream();
	if (gas != 0)
	{
		for (unsigned int i = 0; i < gas; i++)
		{
			char gas = 254;
			ss << left << gas;
		}
		mvaddstr(line++, starting_column, ss.str().c_str());
	}
	else
	{
		ss << "Out of Petrol.";
		mvaddstr(line, starting_column, ss.str().c_str());
	}

}