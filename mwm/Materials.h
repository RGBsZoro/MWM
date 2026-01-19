#pragma once
#include <GL/glut.h>

namespace Materials {

	inline void BMW_Paint_Black() {
		GLfloat ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
		GLfloat diffuse[] = { 0.10f, 0.10f, 0.10f, 1.0f };
		GLfloat specular[] = { 0.9f,  0.9f,  0.9f,  1.0f };
		GLfloat shininess = 96.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}

	inline void BMW_Glass() {
		GLfloat ambient[] = { 0.1f, 0.1f, 0.15f, 0.5f };
		GLfloat diffuse[] = { 0.2f, 0.3f, 0.4f, 0.5f };
		GLfloat specular[] = { 0.9f, 0.9f, 1.0f, 0.5f };
		GLfloat shininess = 120.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}

	inline void Rubber_Tire() {
		GLfloat ambient[] = { 0.02f, 0.02f, 0.02f, 1.0f };
		GLfloat diffuse[] = { 0.05f, 0.05f, 0.05f, 1.0f };
		GLfloat specular[] = { 0.2f,  0.2f,  0.2f,  1.0f };
		GLfloat shininess = 8.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}

	inline void Chrome() {
		GLfloat ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
		GLfloat diffuse[] = { 0.4f,  0.4f,  0.4f,  1.0f };
		GLfloat specular[] = { 1.0f,  1.0f,  1.0f,  1.0f };
		GLfloat shininess = 128.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}
}
