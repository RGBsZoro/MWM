#pragma once
#include "Point.h"

class Road {
	Point start;
	Point end;
	float width;

public:
	Road(Point start, Point end, float width);
	void draw();
};
