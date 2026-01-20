#pragma once
#include "Point.h"
#include "Wheel.h"
#include "Cuboid.h"

class FamilyCar {
public:
    Point pos;
    float carRotation;
    Wheel wheel;
    bool isDriving = false;
    bool headlightsOn = false;
    float carSpeed = 0.0f;  

    float length;
    float width;
    float wheelRadius;
    float bodyHeight;
    bool isDoorOpen = false;
    float doorAngle = 0.0f; 

    FamilyCar(Point p, float l, float w, float r, float h)
        : pos(p), length(l), width(w), wheelRadius(r), bodyHeight(h), carRotation(0.0f) {
    }
    void draw(int r , int g , int b);

    void update() {
        if (isDriving) {
            float angleRad = carRotation * (3.14159f / 180.0f);

            pos.x += cos(angleRad) * carSpeed;
            pos.z -= sin(angleRad) * carSpeed; 

            carSpeed *= 0.99f;
        }
    }
private:
    void drawInterior();
};