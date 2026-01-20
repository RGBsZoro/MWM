#pragma once
#include <vector>
#include "Terrain.h"
#include "Texture.h"
#include "Tree.h"
#include "Garden.h"
#include "CityBlock.h"
#include "TrafficSystem.h"

class World {
    Terrain* terrain;
    Texture groundTex; // تيكستشر الأرض
    std::vector<Garden*> gardens;
    std::vector<CityBlock*> cityBlocks;
    std::vector<Tree*> trees;
    TrafficSystem* traffic;

public:
    World();
    ~World();

    void update();
    void draw();
    void addTree(Tree* t);
    void drawTrees();
};
