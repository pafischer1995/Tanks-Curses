#include <iostream>
#include <sstream>
#include <iomanip>
#include "curses.h"
#include "player.hpp"
#include <locale.h>


using namespace std;

extern int lines;
extern int cols;
extern int base_height_divisor;
extern int max_height_divisor;
extern int ground_type;
extern bool show;
extern bool wind_change;
extern int wind_level;


const int Player::power_increment = 1;
const double Player::angle_increment = 1;


//this is only incremented the first time
Player::Player()
{
	s = LEFT;
	col = 0;
	line = 0;
	angle = 35.0;	//35
	power = 50.0;	//50
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
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	init_pair(8, COLOR_BLACK, COLOR_WHITE);
	init_pair(9, COLOR_BLACK, COLOR_RED);
	init_pair(10, COLOR_BLACK, COLOR_GREEN);
	init_pair(11, COLOR_BLACK, COLOR_YELLOW);

	attron(COLOR_PAIR(color));

	mvaddch(g.ground.at(col) - 1, col + 1, ACS_DIAMOND);

	attroff(COLOR_PAIR(color));

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

	//if the nickname has been changed tell nickname
	if (nick == true)
	{
		ss << setw(10) << left << nickname;
		mvaddstr(line++, starting_column, ss.str().c_str());
	}

	//if it hasn't been changed, give boring version
	else
	{
		ss << setw(10) << left << "Player: " << player;
		mvaddstr(line++, starting_column, ss.str().c_str());
	}

	if (my_turn)
		attroff(A_STANDOUT);

	char credit = 128;

	ss = stringstream();
	ss << setw(10) << left << "Angle: " << setw(6) << angle;
	mvaddstr(line++, starting_column, ss.str().c_str());

	ss = stringstream();
	ss << setw(10) << left << "Power: " << setw(6) << power;
	mvaddstr(line++, starting_column, ss.str().c_str());

	ss = stringstream();
	ss << credit << " [" << points << "]";
	mvaddstr(line++, starting_column, ss.str().c_str());

	ss = stringstream();
	ss << "(P) PointShop";
	move(4, COLS / 2 - 7);
	addstr(ss.str().c_str());

	if (wind_change == true)
	{
		if (wind_level == 1)
		{
			ss = stringstream();
			ss << "<";
			move(2, COLS / 2 - 1);
			addstr(ss.str().c_str());
		}
		else if (wind_level == 2)
		{
			ss = stringstream();
			ss << "< <";
			move(2, COLS / 2 - 1);
			addstr(ss.str().c_str());
		}

		else if (wind_level == 3)
		{
			ss = stringstream();
			ss << "< < <";
			move(2, COLS / 2 - 3);
			addstr(ss.str().c_str());
		}

		else if (wind_level == 4)
		{
			ss = stringstream();
			ss << ">";
			move(2, COLS / 2 - 1);
			addstr(ss.str().c_str());
		}
		else if (wind_level == 5)
		{
			ss = stringstream();
			ss << "> >";
			move(2, COLS / 2 - 1);
			addstr(ss.str().c_str());
		}

		else if (wind_level == 6)
		{
			ss = stringstream();
			ss << "> > >";
			move(2, COLS / 2 - 3);
			addstr(ss.str().c_str());
		}
	}
	else if (wind_change == false)
	{
		ss = stringstream();
		ss << "No Wind";
		move(2, COLS / 2 - 4);
		addstr(ss.str().c_str());
	}


	int color = (bomb_type + 8);
	char square = 'o';

	attron(COLOR_PAIR(color));
	ss = stringstream();
	ss << square;
	mvaddstr(4, starting_column + 10, ss.str().c_str());
	attroff(COLOR_PAIR(color));

	

	attron(COLOR_PAIR(1));

	char h = 254;

	ss = stringstream();
	for (unsigned int i = 0; i < health; i++)
	{	
		ss << h << " ";
	}
	mvaddstr(line++, starting_column, ss.str().c_str());
	attroff(COLOR_PAIR(1));


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

	if (show == true)
	{
		stringstream ss;
		ss = stringstream();
		ss << "Show On";
		move(LINES - 1, COLS - 8);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "ground_type " << ground_type;
		move(0, COLS / 2 - 7);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "bomb_type: " << bomb_type;
		move(0, COLS / 2 - 22);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "B_COL:" << setw(2) << "000 ";
		move(LINES - 1, COLS / 2 - 9);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "B_LINE:" << setw(2) << "00";
		move(LINES - 1, COLS / 2 + 1 );
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "T_LINE:" << setw(3) << "000 ";
		move(LINES - 1, COLS / 2 - 59);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "T_COL: " << setw(2) << "00";
		move(LINES - 1, COLS / 2 - 48);
		addstr(ss.str().c_str());
		
		ss = stringstream();
		ss << wind_level << "]";
		move(0, COLS / 2 + 31);
		addstr(ss.str().c_str());

		ss = stringstream();
		ss << "Turn[" << turn << "] Wind Strength[";
		move(0, COLS / 2 + 9);
		addstr(ss.str().c_str());

		
		refresh();
	}
}