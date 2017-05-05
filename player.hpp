#pragma once
#include "ground.hpp"

extern int gas_toggle;
extern int health_toggle;

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

	bool win_check;								//checks to see if either player has one, based off health in main
	int color = 0;								//starting color is 0. 1-7 are different colors
	int gas = 5 + gas_toggle;					//takes the modified gas value from settings and adds it to gas
	int health = 3 + health_toggle;				//takes the modified heatlh value from settings and adds it to health
	int col;									//starts at 0, modified by initialize
	int line;									//starts at 0, modified by draw
	double power;								//amount of power behind shot
	double angle;								//angle of shot

	Side s;

	static const int power_increment;
	static const double angle_increment;
};