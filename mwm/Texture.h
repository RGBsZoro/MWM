#pragma once
#include <gl/glut.h>
#include "Cuboid.h"
class Texture
{
public:
	GLuint textureID = 0;
	void loadTexture(const char* filename);
	void Use();
};