#pragma once
#include <windows.h>   // يجب أن يكون في الأعلى
#include <cmath>
#include <GL/freeglut.h>
#include "Point.h"
#include "Cuboid.h"
#include "Camera.h"
#include "FamilyCar.h"
#pragma comment(lib, "winmm.lib") // لربط مكتبة الصوت تلقائياً


enum class ElevatorState { IDLE, MOVING_UP, MOVING_DOWN };
enum class DoorState { CLOSED, OPENING, OPEN, CLOSING };

class Elevator {
public:
    Elevator(Point startPos, float targetH);
    Point getPosition() const { return position; }
    float getCabinW() const { return cabinW; }
    float getCabinD() const { return cabinD; }
    float getCabinH() const { return cabinH; }
    float getCurrentY() const { return currentY; }
    void draw();
    //void update(Camera& cam);
    void update(Camera& cam, FamilyCar& car);
    void callElevator(Camera& cam, float backWallZ);

    //bool isAtFloor() const;

private:
    bool isCarInside(FamilyCar& car);
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