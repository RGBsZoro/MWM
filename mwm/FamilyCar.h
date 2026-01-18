#pragma once
#include "Point.h"
#include "Wheel.h"
#include "Cuboid.h"

class FamilyCar {
public:
    Point pos;
    float carRotation;
    Wheel wheel;

    // أبعاد مرنة (Flexible Dimensions)
    float length;
    float width;
    float wheelRadius;
    float bodyHeight;

    // المنشئ الآن يستقبل الأبعاد
    FamilyCar(Point p, float l, float w, float r, float h)
        : pos(p), length(l), width(w), wheelRadius(r), bodyHeight(h), carRotation(0.0f) {
    }
    void draw();
private:
    void drawInterior();
};