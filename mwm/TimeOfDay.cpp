#include "TimeOfDay.h"

TimeOfDay::TimeOfDay() {
	currentMode = NIGHT; // ?????????
}

void TimeOfDay::applyDay() {
	// ????
	glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

	// Ambient ???
	GLfloat ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	// ?????
	glEnable(GL_LIGHT0);
	GLfloat sunPos[] = { 0.0f, 2000.0f, 1000.0f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 0.95f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// ????? ????? ?????
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
}

void TimeOfDay::applyNight() {
	// ???? ?????
	glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

	// Ambient ????
	GLfloat ambient[] = { 0.15f, 0.15f, 0.18f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	// ????? ?????
	glDisable(GL_LIGHT0);

	// ????? ????? ?????
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
}

void TimeOfDay::toggle() {
	currentMode = (currentMode == DAY) ? NIGHT : DAY;
	apply();
}

void TimeOfDay::apply() {
	if (currentMode == DAY)
		applyDay();
	else
		applyNight();
}

TimeMode TimeOfDay::getMode() const {
	return currentMode;
}
