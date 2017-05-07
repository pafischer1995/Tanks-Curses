#pragma once
#include <vector>
#include <string>

class Ground
{
public:
	int ground_type = 0;
	void InitializeGround();
	void Draw();

	std::vector<int> ground;
};
