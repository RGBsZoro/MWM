#pragma once
#include <vector>
#include <GL/freeglut.h>
#include "Point.h"
#include "Cuboid.h"
#include "ModelLoader.h"
#include "CarBMW.h"



class ShowRoom {
public:
    ShowRoom();
    CarBMW& GetBMW();

    void drawJaguar(float x, float y, float z, float rotation);

    void drawSimpleCar(Point p, float scale, float r, float g, float b);

    void drawSportsCar(float x, float y, float z, float rotation);

    void setupPodiumLighting(int id, float x, float z);
    void drawModernPodium(float cx, float cz, int id, int sectionType);
    void drawPodiums(float yOffset); 
    float GetBackWallZ() { return -totalL / 2.0f; }
    void drawCeilingLightRing(Point p);
    void drawOppositeSideline();
    Point GetElevatorShaftCenter() const;
    float GetElevatorDoorWidth() const;
    float GetFloorHeight() const;
    void draw();

    std::vector<Wall> GetStaticWalls();
    Wall GetMainDoorWall();

private:
    CarBMW bmw;
    ModelLoader audiR8;
    ModelLoader jaguar;   

    float floorHeight = 300.0f;
    //void drawElevatorButton(float yOffset, bool isElevatorHere);
    void drawMWMName();
    void drawGlassWallBehindDesk();
    void drawStreet();
    void drawReceptionDesk();
    void drawFloorContent(float yOffset);
    void drawBuildingBase();
    void drawTree(float x, float z);

    float totalW = 1500.0f;
    float totalH = 200.0f;
    float totalL = 1500.0f;
    float doorW = 500.0f;
    float doorH = 150.0f;
};