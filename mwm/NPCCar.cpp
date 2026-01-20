#include "NPCCar.h"
#include "ShowRoom.h"

NPCCar::NPCCar(Point start, float s)
	: position(start), speed(s), rotation(0) {
}

void NPCCar::update() {
	position.x += speed;
	if (position.x > 3000) position.x = -3000;
}

void NPCCar::draw() {
	ShowRoom::drawSimpleCar(
		position,
		0.8f,
		0.2f, 0.2f, 0.8f
	);
}
