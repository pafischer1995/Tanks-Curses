#include "curses.h"
#include "ground.hpp"

//this is where lines, cols, base_height_divisor, and max_heigh_divisor are created
int lines = 40;
int cols = 120;
//base_height_divisor is 8 on original code
int base_height_divisor = 8;
//max_height_divisor is 2 on original code
int max_height_divisor = 2;
extern int ground_type;

//ground was initialized in ground.hpp
void Ground::InitializeGround()
{
	//ground_type == 1 is flat
	//ground_type == 2 is normal
	//ground_type == 3 is higher up

		//int current_height = lines - lines / base_height_divisor;
		//int maximum_height = lines / max_height_divisor;

		int current_height;
		int maximum_height;
		int ground_grow;
		int ground_shrink;

		//higher ground
		if (ground_type == 3)
		{
			current_height = lines - (lines / base_height_divisor) - 10;
			maximum_height = lines / max_height_divisor - 10;
			ground_grow = 1200;
			ground_shrink = 2400;
		}
		//normal ground
		else if (ground_type == 2)
		{
			current_height = lines - lines / base_height_divisor;
			maximum_height = lines / max_height_divisor;
			ground_grow = 800;
			ground_shrink = 1600;
		}
		//flat ground
		else if (ground_type == 1)
		{
			current_height = lines - lines / base_height_divisor -1;
			maximum_height = lines / max_height_divisor;
			ground_grow = -1;
			ground_shrink = -1;
		}

		for (int i = 1; i < cols - 1; i++)
		{

			//this is how far up or down it is
			int h = current_height;
			//this gives 0-10000
			int r = rand() % 10000;
			if (r < ground_grow) {
				//small chace of going up; 0-799 out of 10000
				current_height++;

				//changed so that it is at least 2 from the bottom at all times
				if (current_height >= lines - 3)
					current_height = lines - 3;
			}
			else if (r < ground_shrink)
			{
				//small chace of going down; 800-1599 out of 10000
				current_height--;
				if (current_height < 1)
					current_height = 1;
			}
			ground.push_back(h);
		}
}

void Ground::Draw()
{
	
	//goes from left to right. + rows and cols
	//first 3 are for the first land, next are for the others
	bool first_turn = true;
	while (first_turn == true)
	{
		(ground.at(1) == ground.at(0));
		mvaddch(ground.at(0), 1, '_');
		first_turn = false;
	}

	if (ground.at(1) > ground.at(0))
	{
		mvaddch(ground.at(0), 1, '\\');
	}

	else if (ground.at(1) < ground.at(0))
	{
		mvaddch(ground.at(0), 1, '/');
	}

	else
	{
		(ground.at(1) == ground.at(0));
		mvaddch(ground.at(0), 1, '_');
	}


	for (size_t i = 1; i < ground.size(); i++)
	{
		if (ground.at(i) > ground.at(i - 1))
		{
			mvaddch(ground.at(i), i + 1, '\\');
		}

		else if (ground.at(i) < ground.at(i - 1))
		{
			mvaddch(ground.at(i) + 1, i + 1, '/');
		}

		else if (ground.at(i) == ground.at(i-1))
		{
			mvaddch(ground.at(i), i + 1, '_');
		}
	
	}
}
