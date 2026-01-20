#include "Garden.h"
#include "World.h"
#include <cstdlib>

Garden::Garden(Point min, Point max, int density) {
    for (int i = 0; i < density; i++) {
        float x = min.x + rand() % (int)(max.x - min.x);
        float z = min.z + rand() % (int)(max.z - min.z);

        trees.push_back(
            new Tree(
                Point(x, 0, z),
                TreeType::Default,
                0.8f + (rand() % 40) / 100.0f
            )
        );
    }
}

Garden::~Garden() {
    for (auto t : trees)
        delete t;
}

void Garden::draw() {
    for (auto t : trees)
        t->draw();
}

void Garden::addToWorld(World* world) {
    for (auto t : trees)
        world->addTree(t);
    trees.clear(); 
}
