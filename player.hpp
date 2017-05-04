#pragma once
#include "ground.hpp"
#include <sstream>

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
	void Draw(Ground & ground);
	void Initialize(int column, Side side);
	void DrawSettings(int turn);
	void PowerUp();
	void PowerDown();
	void AngleUp();
	void AngleDown();
	
	int nickname;
	bool win_check;
	int gas = 5 + gas_toggle;
	int health = 3 + health_toggle;
	int col;
	int line;
	double power;
	double angle;
	
	Side s;

	static const int power_increment;
	static const double angle_increment;
};
