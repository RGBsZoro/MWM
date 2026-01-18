#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Wheel {
public:
    void draw(float radius, float width) {
        GLUquadric* q = gluNewQuadric();

        // 1. الإطار الخارجي (أسود)
        glColor3ub(20, 20, 20);
        gluCylinder(q, radius, radius, width, 30, 30);

        // 2. وجه الجنط الخارجي (فضي)
        glPushMatrix();
        glTranslatef(0, 0, width);
        glColor3ub(170, 170, 170);
        gluDisk(q, 0, radius, 30, 1);
        glPopMatrix();

        // 3. وجه الجنط الداخلي (أسود)
        glPushMatrix();
        glColor3ub(10, 10, 10);
        gluDisk(q, 0, radius, 30, 1);
        glPopMatrix();

        gluDeleteQuadric(q);
    }
};