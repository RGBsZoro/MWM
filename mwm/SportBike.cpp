#include "SportBike.h"

SportBike::SportBike(Point p) : Bike(p, 0.8f, 0.0f, 0.0f) {} // لون أحمر رياضي

void SportBike::draw() {
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);

    glTranslatef(pos.x, pos.y + 12.0f, pos.z);

    Cylinder cyl;

    // --- العجلات العريضة جداً ---
    glColor3f(0.05f, 0.05f, 0.05f);
    for (float zOff : {-50.0f, 50.0f}) {
        glPushMatrix();
        glTranslatef(0, 20, zOff);
        glRotatef(90, 0, 1, 0);
        cyl.draw(Point(0, 0, 0), 20, 20, 12, 30, 1); // عجلات عريضة (12)
        glPopMatrix();
    }

    // --- الجسم الانسيابي (Fairing) ---
    glColor3f(red, green, blue);
    Cuboid(Point(0, 35, 0), 25, 35, 100).draw(); // الجسم الرئيسي

    // خزان الوقود (مرتفع ومنحني)
    glPushMatrix();
    glTranslatef(0, 50, 10);
    glScalef(1.2f, 1.0f, 1.4f);
    Cuboid(Point(0, 0, 0), 15, 30, 40).draw();
    glPopMatrix();

    // --- المحرك (Engine) - تفاصيل ميكانيكية أسفل الجسم ---
    glColor3f(0.3f, 0.3f, 0.3f);
    Cuboid(Point(0, 20, 0), 20, 25, 45).draw();

    // --- العادم (Exhaust) ---
    glColor3f(0.6f, 0.6f, 0.6f);
    glPushMatrix();
    glTranslatef(18, 15, -40);
    glRotatef(-10, 1, 0, 0);
    cyl.draw(Point(0, 0, 0), 5, 4, 40, 20, 1);
    glPopMatrix();

    // --- كشاف الإضاءة الأمامي ---
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.9f);
    Cuboid(Point(0, 45, 52), 8, 15, 2).draw();
    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    glPopMatrix();
}