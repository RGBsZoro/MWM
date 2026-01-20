#include "Tree.h"
#include "Cuboid.h"

Tree::Tree(Point p, TreeType t, float s)
    : position(p), type(t), scale(s) {
}

void Tree::draw() {
    glPushMatrix();
    glTranslatef(position.x, 0.0f, position.z);
    glScalef(scale, scale, scale);

    glColor3ub(101, 67, 33);
    Cuboid trunk(Point(0, 0.0f, 0), 50.0f, 12.0f, 12.0f);
    trunk.draw();

    glColor3ub(34, 139, 34);
    Cuboid leaves1(Point(0, 45.0f, 0), 40.0f, 70.0f, 70.0f); 
    leaves1.draw();

    glColor3ub(46, 170, 46);
    Cuboid leaves2(Point(0, 75.0f, 0), 35.0f, 55.0f, 55.0f); 
    leaves2.draw();

    glColor3ub(60, 200, 60);
    Cuboid leaves3(Point(0, 100.0f, 0), 25.0f, 35.0f, 35.0f); 
    leaves3.draw();

    glPopMatrix();
}
