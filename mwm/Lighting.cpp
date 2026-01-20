#include "Lighting.h"

#define M_PI 3.14159265358979323846

inline double toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

void Lighting::setupLighting(Point lightPosition, int lightIndex)
{
    GLenum lightID = GL_LIGHT0 + lightIndex;

    LightPos[0] = lightPosition.x;
    LightPos[1] = lightPosition.y;
    LightPos[2] = lightPosition.z;
    LightPos[3] = 1.0f; // ضوء موضعي (Point Light)

    LightAmb[0] = 0.1f; LightAmb[1] = 0.1f; LightAmb[2] = 0.1f; LightAmb[3] = 1.0f;
    LightDiff[0] = 1.0f; LightDiff[1] = 0.9f; LightDiff[2] = 0.5f; LightDiff[3] = 1.0f;
    LightSpec[0] = 1.0f; LightSpec[1] = 1.0f; LightSpec[2] = 1.0f; LightSpec[3] = 1.0f;

    glEnable(lightID);
    glLightfv(lightID, GL_POSITION, LightPos);
    glLightfv(lightID, GL_AMBIENT, LightAmb);
    glLightfv(lightID, GL_DIFFUSE, LightDiff);
    glLightfv(lightID, GL_SPECULAR, LightSpec);

    glLightf(lightID, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(lightID, GL_LINEAR_ATTENUATION, 0.005f); // اجعل الضوء يصل للأرض
    glLightf(lightID, GL_QUADRATIC_ATTENUATION, 0.0f);

    MatAmb[0] = 0.2f; MatAmb[1] = 0.2f; MatAmb[2] = 0.2f; MatAmb[3] = 1.0f;
    MatDif[0] = 0.8f; MatDif[1] = 0.8f; MatDif[2] = 0.8f; MatDif[3] = 1.0f;
    MatSpec[0] = 1.0f; MatSpec[1] = 1.0f; MatSpec[2] = 1.0f; MatSpec[3] = 1.0f;
    MatShn[0] = 100.0f;

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, MatShn);
}

void Lighting::drawStreetLight(Point baseCenter, double poleHeight, double poleRadius, double curveRadius, double curveAngle, double lampSize, bool isLeftSide, int lightIndex)
{
    GLUquadric* quad = gluNewQuadric();

    glColor3ub(60, 60, 60); // رمادي معدني
    glPushMatrix();
    glTranslated(baseCenter.x, baseCenter.y, baseCenter.z);
    glRotated(-90, 1, 0, 0);
    gluCylinder(quad, poleRadius, poleRadius * 0.7, poleHeight, 32, 32);
    glPopMatrix();

    double direction = isLeftSide ? 1.0 : -1.0;

    Point topOfPole(baseCenter.x, baseCenter.y + poleHeight, baseCenter.z);

    Point lampPosition;
    lampPosition.x = topOfPole.x + (curveRadius * direction);
    lampPosition.y = topOfPole.y + (curveRadius * 0.3); // مائل للأعلى قليلاً
    lampPosition.z = topOfPole.z;

    glPushMatrix();
    glTranslated(topOfPole.x, topOfPole.y, topOfPole.z);
    if (!isLeftSide) glRotated(180, 0, 1, 0); // قلب الاتجاه إذا لم يكن يساراً
    glRotated(90, 0, 1, 0);  // توجيه للجانب
    glRotated(-20, 1, 0, 0); // زاوية ميل الذراع للأعلى
    gluCylinder(quad, poleRadius * 0.7, poleRadius * 0.3, curveRadius * 1.1, 16, 16);
    glPopMatrix();

    this->setupLighting(lampPosition, lightIndex);

    glPushMatrix();
    glTranslated(lampPosition.x, lampPosition.y, lampPosition.z);
    glDisable(GL_LIGHTING);
    glColor3ub(255, 255, 200); // أبيض مائل للصفرة (متوهج)
    glutSolidSphere(lampSize, 20, 20);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glColor3ub(255, 255, 255);
    gluDeleteQuadric(quad);
}