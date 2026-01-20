#pragma once
#include <vector>
#include "Building.h"

class CityBlock {
	std::vector<Building> buildings;

public:
	CityBlock(Point center, int rows, int cols);
	void draw();
};
