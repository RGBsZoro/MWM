#include "Terrain.h"

Terrain::Terrain(float s, float yPos, int t)
    : size(s), y(yPos), type(t) {
}
void Terrain::draw(Texture& tex) {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    tex.Use();

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor3f(1.0f, 1.0f, 1.0f);

    float s = 8000.0f;    // كبّرنا المساحة لتغطي الأفق (الحديقة)
    float repeat = 50.0f; // زدنا التكرار لكي لا تظهر البكسلات كبيرة وتكون التفاصيل ناعمة

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);       glVertex3f(-s, y, -s);
    glTexCoord2f(repeat, 0);  glVertex3f(s, y, -s);
    glTexCoord2f(repeat, repeat); glVertex3f(s, y, s);
    glTexCoord2f(0, repeat);  glVertex3f(-s, y, s);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}
