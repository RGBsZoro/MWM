#pragma once
#include <vector>
#include "NPCCar.h"

class TrafficSystem {
	std::vector<NPCCar> cars;

public:
	TrafficSystem(int count);
	void update();
	void draw();
};
