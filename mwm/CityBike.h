#pragma once
#include "Bike.h"

class CityBike : public Bike {
public:
    CityBike(Point p);
    void draw() override;
};