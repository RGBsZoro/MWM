#pragma once
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
    void toggle();

    bool isAtFloor() const;

private:
    bool isPlayerInside(Camera& cam);

private:
    Point position;

    float bottomY;
    float topY;
    float currentY;
    float speed;

    DoorState doorState;
    float doorWidth;
    float maxDoorWidth;

    ElevatorState state;
};
