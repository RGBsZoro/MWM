#pragma once
#include <GL/glut.h>
#include "Texture.h"

enum TerrainType {
    Grass = 0,
    Sand = 1,
    Road = 2
};

class Terrain {
    float size;
    float y;
    int type;

public:
    Terrain(float s, float yPos, int t);

    void draw(Texture& tex);
};
