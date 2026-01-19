#pragma once
#include <cmath>
#include <GL/freeglut.h>
#include "Point.h"
#include "Cuboid.h"
#include "Camera.h"

enum class ElevatorState { IDLE, MOVING_UP, MOVING_DOWN };
enum class DoorState { CLOSED, OPENING, OPEN, CLOSING };

class Elevator {
public:
    Elevator(Point startPos, float targetH);

    void draw();
    void update(Camera& cam);
    void callElevator(Camera& cam, float backWallZ);

    bool isAtFloor() const;

private:
    bool isPlayerInside(Camera& cam);
    void drawCeilingLight();
    void drawControlPanel();
    void drawFloorIndicator();

private:
    Point position;

    float bottomY;
    float topY;
    float currentY;
    float speed;

    // أبعاد مصعد سيارات
    float cabinW;
    float cabinD;
    float cabinH;

    DoorState doorState;
    float doorWidth;
    float maxDoorWidth;

    ElevatorState state;
};
