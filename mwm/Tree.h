#pragma once
#include "Point.h"
#include "Cuboid.h"

enum class TreeType { Default, Palm };

class Tree {
public:
    Point position;
    TreeType type;
    float scale;

    Tree(Point p, TreeType t, float s = 1.0f);
    void draw();
};
