#pragma once
#include "Point.h"
#include "Cuboid.h"
#include "Cylinder.h"
#include <GL/freeglut.h>

class Bike {
protected:
    Point pos;
    float red, green, blue;

public:
    Bike(Point p, float r, float g, float b) : pos(p), red(r), green(g), blue(b) {}
    virtual ~Bike() {}

    // دالة افتراضية تجعل الكلاس Polymorphic
    virtual void draw() = 0;
};