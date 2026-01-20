#include "World.h"
#include <iostream>

World::World() {
    terrain = new Terrain(6000, -3.2f, TerrainType::Grass);

    // 5تحميل التيكستشر الأرض
    groundTex.loadTexture(R"(C:\Users\Massa\source\repos\MWM\mwm\Textures/garden.jpg)");

    gardens.push_back(new Garden(Point(-2500, 0, 1500), Point(2500, 0, 2600), 40));
    cityBlocks.push_back(new CityBlock(Point(0, 0, -3000), 3, 5));

    traffic = new TrafficSystem(6);

    for (auto g : gardens)
        g->addToWorld(this);
}

World::~World() {
    delete terrain;
    delete traffic;

    for (auto g : gardens) delete g;
    for (auto c : cityBlocks) delete c;
    for (auto t : trees) delete t;
}

void World::update() {
    traffic->update();
}

void World::draw() {
    // رسم الأرضية بالتيكستشر
    glDisable(GL_LIGHTING);
    // تأكد من إضافة السطرين التاليين هنا قبل الرسم مباشرة
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    terrain->draw(groundTex);

    glEnable(GL_LIGHTING);

    for (auto c : cityBlocks)
        c->draw();

    drawTrees();

    traffic->draw();
}

void World::addTree(Tree* t) {
    t->position.y = 0.0f;
    trees.push_back(t);
}

void World::drawTrees() {
    for (auto t : trees)
        t->draw();
}
