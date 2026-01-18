#pragma once
#include <vector>
#include <GL/freeglut.h>
#include "Point.h"
#include "Cuboid.h"
#include "Types.h" // الذي أنشأناه سابقاً

class ShowRoom {
public:
    ShowRoom();
    
    void setupPodiumLighting(int id, float x, float z);
    void drawModernPodium(float cx, float cz, int id);
    void drawPodiums();
    float GetBackWallZ() { return -totalL / 2.0f; }
    void drawCeilingLightRing(Point p);
    void drawOppositeSideline();
    Point GetElevatorShaftCenter() const;
    float GetElevatorDoorWidth() const;
    float GetFloorHeight() const;
    //void drawSinglePodium(float x, float z, float w, float l, color3f col);
    void draw(); // لرسم كل شيء (المبنى + الشارع)

    // دوال لإرجاع إحداثيات التصادم للكاميرا
    std::vector<Wall> GetStaticWalls();
    Wall GetMainDoorWall();

private:
    float elevatorY = -3.0f;       // الارتفاع الحالي للمصعد
    bool movingToTop = false;      // هل المصعد متوجه للأعلى؟
    float floorHeight = 300.0f;
    void drawElevatorButton(float yOffset, bool isElevatorHere);
    // ارتفاع الطابق الثاني (المسافة بين الأرضيتين)
    void drawMWMName();
    void drawStreet();
   

    //void drawBuilding();
    //void drawPodiums(float x, float z, float w, float l);
    //void drawCeilingLights();
    void drawReceptionDesk();
    void drawFloorContent(float yOffset);
    //void drawElevator();
    //void drawOffice();
    void drawBuildingBase();

    void drawTree(float x, float z);

    // قياسات المعرض
    float totalW = 1500.0f;
    float totalH = 200.0f;
    float totalL = 1500.0f;
    float doorW = 500.0f;
    float doorH = 150.0f;
};