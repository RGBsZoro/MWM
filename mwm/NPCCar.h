#pragma once
#include "Point.h"

class NPCCar {
	Point position;
	float speed;
	float rotation;

public:
	NPCCar(Point start, float speed);
	void update();
	void draw();
};
