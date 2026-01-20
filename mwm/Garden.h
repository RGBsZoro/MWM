#pragma once
#include <vector>
#include "Tree.h"

class World; // forward declaration

class Garden {
    std::vector<Tree*> trees;

public:
    Garden(Point min, Point max, int density);
    ~Garden();

    void draw();
    void addToWorld(World* world);
};
