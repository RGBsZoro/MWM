#pragma once
#include "Point.h"
#include <GL/glut.h>
#include <string>
#include "Texture.h"
#include "Types.h"  // ????? ??????? ????????
#include <GL/freeglut.h>

class Cuboid {
public:
    Cuboid(Point center, double height, double length, double width);

    void draw();
    void drawWithTexture(GLuint textureID, int repeatX, int repeatY);
    void drawWithTextureNoTopBottom(GLuint textureID, int repeatX, int repeatY);
    void drawWithTextureOnOneFace(GLuint textureID, const std::string& face, int repeatX, int repeatY);
    Wall ToWall() const;

private:
    Point center;
    double height;
    double length;
    double width;
};
