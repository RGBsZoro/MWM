#include "ElectricBike.h"

ElectricBike::ElectricBike(Point p) : Bike(p, 0.0f, 0.8f, 0.2f) {} // لون أخضر

void ElectricBike::draw() {
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glTranslatef(pos.x, pos.y + 15.0f, pos.z);

    Cylinder cyl;

    // --- العجلات ---
    glColor3f(0.1f, 0.1f, 0.1f);
    for (float zOff : {-45.0f, 45.0f}) {
        glPushMatrix();
        glTranslatef(0, 18, zOff);
        glRotatef(90, 0, 1, 0);
        cyl.draw(Point(0, 0, 0), 19, 19, 5, 30, 1);
        glPopMatrix();
    }

    // --- صندوق البطارية (Electric Core) ---
    // هذا الجزء هو ما يميز الدراجة الكهربائية
    glColor3f(0.2f, 0.2f, 0.2f); // لون داكن للبطارية
    Cuboid(Point(0, 30, 5), 35, 15, 50).draw();

    // إضافة خطوط إضاءة (Neon) على البطارية
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 1.0f); // لون أزرق سماوي "كهربائي"
    Cuboid(Point(8, 30, 5), 2, 16, 40).draw();
    glEnable(GL_LIGHTING);

    // --- الهيكل العلوي ---
    glColor3f(red, green, blue);
    Cuboid(Point(0, 42, 0), 4, 5, 85).draw(); // أنبوب علوي مستقيم

    // --- المقعد المستقبلي ---
    glColor3f(0.1f, 0.1f, 0.1f);
    Cuboid(Point(0, 48, -20), 3, 25, 10).draw();

    glPopMatrix();
}