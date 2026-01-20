#include "TrafficSystem.h"

TrafficSystem::TrafficSystem(int count) {
	for (int i = 0; i < count; i++) {
		cars.emplace_back(Point(-2000 + i * 400, 0, 900), 2.0f);
	}
}

void TrafficSystem::update() {
	for (auto& c : cars)
		c.update();
}

void TrafficSystem::draw() {
	for (auto& c : cars)
		c.draw();
}
