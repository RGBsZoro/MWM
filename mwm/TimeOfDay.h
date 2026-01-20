#pragma once
#ifndef TIME_OF_DAY_H
#define TIME_OF_DAY_H

#include <GL/freeglut.h>

enum TimeMode {
	DAY,
	NIGHT
};

class TimeOfDay {
private:
	TimeMode currentMode;

	void applyDay();
	void applyNight();

public:
	TimeOfDay();

	void toggle();
	void apply();          // تطبّق الوضع الحالي
	TimeMode getMode() const;
};

#endif
