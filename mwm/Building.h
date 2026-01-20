#pragma once
#include "Point.h"

class Building {
	Point position;
	float width;
	float depth;
	float height;

public:
	Building(Point pos, float w, float d, float h);
	void draw();
};
