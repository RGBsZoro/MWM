#pragma once
#include "Bike.h"

class ElectricBike : public Bike {
public:
    ElectricBike(Point p);
    void draw() override;
};