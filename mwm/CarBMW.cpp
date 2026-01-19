#include "CarBMW.h"
#include <cmath>
#include "Materials.h"
#include <windows.h>
#include <mmsystem.h>
#include <iostream>

// ربط مكتبة الصوت الخاصة بويندوز
#pragma comment(lib, "winmm.lib")

// 1. الكونستركتور (Constructor)
CarBMW::CarBMW(Point pos, const char* soundPath)
    : position(pos), rotation(0), wheelAngle(0), hornSoundPath(soundPath) {
    grillTex.loadTexture("Textures/bmw_grill.jpg");
    rimTex.loadTexture("Textures/bmw_rim.jpg");
}

// 2. ميثود تشغيل الزمور (Play Horn)
void CarBMW::playHorn(Point cameraPos) {
    float dx = position.x - cameraPos.x;
    float dy = position.y - cameraPos.y;
    float dz = position.z - cameraPos.z;
    float distance = sqrt(dx * dx + dy * dy + dz * dz);

    // ... حساب المسافة كما فعلنا سابقاً ...
    if (distance < 500.0f) {
        PlaySoundA(hornSoundPath, NULL, SND_FILENAME | SND_ASYNC);

        // تغيير لون الأضواء للحظة (تأثير بصري)
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 1.0f); // أبيض قوي جداً
        drawLights(); // استدعاء رسم الأضواء داخل الميثود
        glEnable(GL_LIGHTING);
    }
}

// 3. دالة الرسم الرئيسية (Draw)
void CarBMW::draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation, 0, 1, 0);

    // تصغير السيارة لتناسب حجم المعرض
    glScalef(0.25f, 0.25f, 0.25f);

    drawBody();
    drawRoof();
    drawWindows();
    drawExtraDetails();
    drawGrill();
    drawLights();
    drawWheels();

    glPopMatrix();
}

// 4. جسم السيارة (Body)
void CarBMW::drawBody() {
    Materials::BMW_Paint_Black();

    // الجسم الرئيسي
    glPushMatrix();
    glTranslatef(0, 45, 0);
    drawCuboid(420, 70, 180);
    glPopMatrix();

    // الصدام الأمامي (Front Bumper) لمزيد من التفاصيل
    glPushMatrix();
    glTranslatef(205, 30, 0);
    drawCuboid(20, 40, 185);
    glPopMatrix();
}

// 5. السقف (Roof)
void CarBMW::drawRoof() {
    Materials::BMW_Paint_Black();
    glPushMatrix();
    glTranslatef(-20, 95, 0);
    drawCuboid(240, 40, 150);
    glPopMatrix();
}

// 6. النوافذ المحسنة (Windows)
void CarBMW::drawWindows() {
    // رسم إطار أسود مطفي للنوافذ ليعطي شكل واقعي
    Materials::Rubber_Tire();
    glPushMatrix();
    glTranslatef(-20, 95, 0);
    drawCuboid(242, 42, 152);
    glPopMatrix();

    // رسم الزجاج مع شفافية
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Materials::BMW_Glass();

    glPushMatrix();
    glTranslatef(-20, 95, 0);
    drawCuboid(238, 38, 154); // الزجاج يبرز قليلاً للخارج من الجوانب
    glPopMatrix();

    glDisable(GL_BLEND);
}

// 7. العجلات (Wheels)
void CarBMW::drawWheels() {
    GLUquadric* q = gluNewQuadric();
    gluQuadricTexture(q, GL_TRUE);
    gluQuadricNormals(q, GLU_SMOOTH);

    float x[4] = { 140, -140, 140, -140 };
    float z[4] = { 95, 95, -120, -120 }; // إزاحة لتبرز خارج الجسم

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(x[i], 30, z[i]);
        glRotatef(wheelAngle, 0, 0, 1);

        // الإطار المطاطي
        Materials::Rubber_Tire();
        gluCylinder(q, 32, 32, 25, 32, 1);

        // الجنط التكستشر
        Materials::Chrome();
        rimTex.Use();
        glPushMatrix();
        glTranslatef(0, 0, 25);
        glNormal3f(0, 0, 1);
        gluDisk(q, 0, 32, 32, 1);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    gluDeleteQuadric(q);
}

// 8. التفاصيل الإضافية (Extra Details)
void CarBMW::drawExtraDetails() {
    // المرايا الجانبية
    Materials::BMW_Paint_Black();
    // يمين
    glPushMatrix();
    glTranslatef(100, 85, 95);
    drawCuboid(15, 10, 25);
    glPopMatrix();
    // يسار
    glPushMatrix();
    glTranslatef(100, 85, -95);
    drawCuboid(15, 10, 25);
    glPopMatrix();

    // لوحة الأرقام (License Plate)
    glDisable(GL_LIGHTING);
    glColor3f(0.9f, 0.9f, 0.9f);
    glPushMatrix();
    glTranslatef(216, 30, 0);
    drawCuboid(2, 12, 45);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// 9. الأضواء (Lights)
void CarBMW::drawLights() {
    glDisable(GL_LIGHTING); // لجعلها تبدو مضيئة ذاتياً
    glColor3f(1.0f, 1.0f, 0.8f);

    glPushMatrix();
    glTranslatef(211, 60, 55);
    drawCuboid(5, 15, 40); // ضوء يمين
    glTranslatef(0, 0, -110);
    drawCuboid(5, 15, 40); // ضوء يسار
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

// 10. الشبكة الأمامية (Grill)
void CarBMW::drawGrill() {
    Materials::Chrome();
    grillTex.Use();
    glPushMatrix();
    glTranslatef(211, 50, 0);
    float h = 20, d = 35;
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(0.5, -h, -d);
    glTexCoord2f(1, 0); glVertex3f(0.5, h, -d);
    glTexCoord2f(1, 1); glVertex3f(0.5, h, d);
    glTexCoord2f(0, 1); glVertex3f(0.5, -h, d);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

// 11. دالة رسم المكعب الأساسية (Draw Cuboid)
void CarBMW::drawCuboid(float w, float h, float d) {
    w /= 2; h /= 2; d /= 2;
    glBegin(GL_QUADS);
    // Front
    glNormal3f(0, 0, 1);
    glVertex3f(w, -h, d); glVertex3f(w, h, d);
    glVertex3f(-w, h, d); glVertex3f(-w, -h, d);
    // Back
    glNormal3f(0, 0, -1);
    glVertex3f(w, -h, -d); glVertex3f(-w, -h, -d);
    glVertex3f(-w, h, -d); glVertex3f(w, h, -d);
    // Right
    glNormal3f(1, 0, 0);
    glVertex3f(w, -h, -d); glVertex3f(w, h, -d);
    glVertex3f(w, h, d); glVertex3f(w, -h, d);
    // Left
    glNormal3f(-1, 0, 0);
    glVertex3f(-w, -h, -d); glVertex3f(-w, -h, d);
    glVertex3f(-w, h, d); glVertex3f(-w, h, -d);
    // Top
    glNormal3f(0, 1, 0);
    glVertex3f(w, h, d); glVertex3f(w, h, -d);
    glVertex3f(-w, h, -d); glVertex3f(-w, h, d);
    // Bottom
    glNormal3f(0, -1, 0);
    glVertex3f(w, -h, d); glVertex3f(-w, -h, d);
    glVertex3f(-w, -h, -d); glVertex3f(w, -h, -d);
    glEnd();
}