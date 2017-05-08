#pragma once
#include "ground.hpp"



enum Side
{
	LEFT,
	RIGHT
};

class Player
{
public:
	Player();
	void Draw(Ground & ground);					//
	void Initialize(int column, Side side);		//this assigns each player a color and side
	void DrawSettings(int turn);				//
	void PowerUp();								//increments power up by one
	void PowerDown();							//increments power down by one
	void AngleUp();								//increments angle up by one
	void AngleDown();							//increments angle down by one

	bool nick = false;
	char * nickname;
	bool win_check;								//checks to see if either player has one, based off health in main
	int bomb_type = 0;							//bomb type 0 is regular bomb, 1 is strong bomb, 2 is large bomb
	int color = 0;								//starting color is 0. 1-7 are different colors
	int gas = 0;								
	int health = 0;			//takes the modified heatlh value from settings and adds it to health
	int points = 0;
	int col;									//starts at 0, modified by initialize
	int line;									//starts at 0, modified by draw
	double power;								//amount of power behind shot
	double angle;								//angle of shot
	double ws = 0.0;

	Side s;

	static const int power_increment;
	static const double angle_increment;
};