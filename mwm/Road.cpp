#include "Road.h"
#include "Cuboid.h"

Road::Road(Point s, Point e, float w)
	: start(s), end(e), width(w) {
}

void Road::draw() {
	glColor3ub(25, 25, 25);

	float length = fabs(end.z - start.z);
	Point center(
		(start.x + end.x) / 2,
		0.0f,
		(start.z + end.z) / 2
	);

	Cuboid(center, 1.0f, width, length).draw();
}
