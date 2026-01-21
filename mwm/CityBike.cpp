#include "CityBike.h"

CityBike::CityBike(Point p) : Bike(p, 0.1f, 0.4f, 0.9f) {} // لون أزرق

void CityBike::draw() {
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);

    // رفع الدراجة فوق المنصة قليلاً
    glTranslatef(pos.x, pos.y + 15.0f, pos.z);
    glScalef(0.8f, 0.8f, 0.8f);

    Cylinder c;

    // 1. العجلات (عجلة أمامية وخلفية)
    glColor3f(0.1f, 0.1f, 0.1f); // أسود للإطارات
    float wheelZ[2] = { -45.0f, 45.0f }; // مواقع العجلتين
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(0, 15, wheelZ[i]);
        glRotatef(0, 0, 1, 0); // تدوير العجلة لتكون باتجاه الحركة
        c.draw(Point(0, 0, 0), 18, 18, 4, 30, 1);
        glPopMatrix();
    }

    // 2. الهيكل الرئيسي (أنابيب)
    glColor3f(red, green, blue);

    // أنبوب القاعدة: يربط بين العجلتين
    glPushMatrix();
    glTranslatef(0, 15, 0);
    glRotatef(90, 1, 0, 0);
    Cuboid(Point(0, 0, -45), 3, 3, 90).draw();
    glPopMatrix();

    // أنبوب المقعد: مائل قليلاً من منتصف الأنبوب السفلي إلى المقعد
    glPushMatrix();
    glTranslatef(0, 15, 0); // نقطة البداية عند منتصف الأنبوب السفلي
    glRotatef(-20, 1, 0, 0); // ميل نحو المقعد
    Cuboid(Point(0, 0, 0), 3, 3, 30).draw();
    glPopMatrix();

    // أنبوب المقود الأمامي
    glPushMatrix();
    glTranslatef(0, 15, 45); // أمام الدراجة
    glRotatef(45, 1, 0, 0); // زاوية للاتجاه الأمامي
    Cuboid(Point(0, 0, 0), 3, 3, 20).draw();
    glPopMatrix();

    // 3. المقعد
    glColor3f(0.2f, 0.2f, 0.2f);
    Cuboid(Point(0, 35, -22), 3, 15, 10).draw();

    // 4. المقود العرضي
    glPushMatrix();
    glTranslatef(0, 35, 50); // فوق الأنبوب الأمامي
    glColor3f(0.7f, 0.7f, 0.7f); // لون كروم/فضي
    Cuboid(Point(0, 0, 0), 2, 40, 2).draw(); // الأنبوب العرضي
    glPopMatrix();

    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}
