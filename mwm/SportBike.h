#pragma once
#include "Bike.h"

class SportBike : public Bike {
public:
    SportBike(Point p);
    void draw() override;
};