#pragma once
#include "Point.h"
#include "Wheel.h"
#include "Cuboid.h"

class FamilyCar {
public:
    Point pos;
    float carRotation;
    Wheel wheel;
    bool isDriving = false; // هل المستخدم داخل السيارة؟
    float carSpeed = 0.0f;  // 

    // أبعاد مرنة (Flexible Dimensions)
    float length;
    float width;
    float wheelRadius;
    float bodyHeight;
    bool isDoorOpen = false;
    float doorAngle = 0.0f; // 0 مغلق، 90 مفتوح مثلاً

    // المنشئ الآن يستقبل الأبعاد
    FamilyCar(Point p, float l, float w, float r, float h)
        : pos(p), length(l), width(w), wheelRadius(r), bodyHeight(h), carRotation(0.0f) {
    }
    void draw();

    void update() {
        if (isDriving) {
            // تحويل الزاوية إلى راديان
            float angleRad = carRotation * (3.14159f / 180.0f);

            // التعديل: الحركة باتجاه X لأن بوز السيارة باتجاه X في كود الرسم
            pos.x += cos(angleRad) * carSpeed;
            pos.z -= sin(angleRad) * carSpeed; // المحور Z معكوس في OpenGL بالنسبة للدوران

            carSpeed *= 0.98f;
        }
    }
private:
    void drawInterior();
};