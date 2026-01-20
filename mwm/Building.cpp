#include "Building.h"
#include "Cuboid.h"

Building::Building(Point p, float w, float d, float h)
	: position(p), width(w), depth(d), height(h) {
}

void Building::draw() {
	float scale = 1.5f; // ????? ?????
	float w = width * scale;
	float d = depth * scale;
	float h = height * scale;

	glColor3ub(120, 120, 120);
	Cuboid(Point(position.x, 0.0f, position.z), h, d, w).draw();

	glColor3ub(80, 80, 80);
	Cuboid(Point(position.x, h + 2, position.z), 4, d + 10, w + 10).draw();
}

