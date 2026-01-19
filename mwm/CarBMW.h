#pragma once
#include <GL/glut.h>
#include "Point.h"
#include "Texture.h"

class CarBMW {
public:
    const char* hornSoundPath; // متغير لتخزين مسار صوت الزمور

    CarBMW(Point pos, const char* soundPath); // تعديل الكونستركتور ليقبل الصوت
    void playHorn(Point cameraPos);
    Texture grillTex;
    Texture rimTex;
    Point position;
    float rotation;
    float wheelAngle;

    void draw(); // تأكد من وجود هذه الدالة هنا

private:
    void drawBody();
    void drawRoof();
    void drawWindows();
    void drawExtraDetails();
    void drawWheels();
    void drawLights();
    void drawGrill();
    void drawCuboid(float w, float h, float d); // تأكد من تطابق البارامترات
};