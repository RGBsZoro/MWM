#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // لربط مكتبة الوسائط المتعددة بويندوز
#include <GL/freeglut.h>
#include <iostream>
#include "Point.h"
#include "Cuboid.h"
#include "Camera.h"
#include "Door.h"
#include "Lighting.h"
#include "ShowRoom.h"
#include "Elevator.h"

#include "FamilyCar.h"
#ifndef GL_LIGHT1
#define GL_LIGHT0 0x4000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#endif

using namespace std;

// المتغيرات العامة
CameraMode currentCamera = FREE;
Camera camera;
Door mainDoor;
FamilyCar tahoe(Point(100, 0, 1000), 140.0f, 65.0f, 14.0f, 50.0f);
Lighting sceneLighting;
ShowRoom mwmShowroom;
Point shaftPos = mwmShowroom.GetElevatorShaftCenter();

Elevator myElevator(shaftPos, mwmShowroom.GetFloorHeight());
//CarBMW bmwCar(Point(0, -3.0f, 300), "Sounds/car-not-starting.wav");
GLuint displayListID;
bool g_mouseCaptured = false;
int g_lastMouseX = 400;
int g_lastMouseY = 300;
float g_mouseSensitivity = 0.0015f; 

int g_iWidth = 1024;
int g_iHeight = 768;
const float g_fNear = 1.0f;
const float g_fFar = 15000.0f;

void setupCollision() {
	camera.walls.clear();
	camera.doorWalls.clear();
	camera.Doors.clear();

	camera.walls = mwmShowroom.GetStaticWalls();

	camera.Doors.push_back(&mainDoor);
	camera.doorWalls.push_back(mwmShowroom.GetMainDoorWall());
}

void drawGround() {
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.1f, 0.1f, 0.1f); 
	glBegin(GL_QUADS);
	glVertex3f(-5000.0f, -3.1f, -5000.0f);
	glVertex3f(5000.0f, -3.1f, -5000.0f);
	glVertex3f(5000.0f, -3.1f, 5000.0f);
	glVertex3f(-5000.0f, -3.1f, 5000.0f);
	glEnd();
	glEnable(GL_LIGHTING);
}

void drawTree(float x, float z) {
	glColor3ub(101, 67, 33);
	Cuboid trunk(Point(x, 0.0f, z), 50.0f, 12.0f, 12.0f);
	trunk.draw();


	glColor3ub(34, 139, 34);
	Cuboid leaves1(Point(x, 45.0f, z), 40.0f, 70.0f, 70.0f);
	leaves1.draw();

	glColor3ub(46, 170, 46);
	Cuboid leaves2(Point(x, 75.0f, z), 35.0f, 55.0f, 55.0f);
	leaves2.draw();

	glColor3ub(60, 200, 60);
	Cuboid leaves3(Point(x, 100.0f, z), 25.0f, 35.0f, 35.0f);
	leaves3.draw();
}

void setDriverSeatCamera(CarBMW& car) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float rad = car.rotation * 3.14159f / 180.0f;

	Point camLocal(-40, 95, 35);

	float camX = car.position.x +
		camLocal.x * cos(rad) - camLocal.z * sin(rad);
	float camZ = car.position.z +
		camLocal.x * sin(rad) + camLocal.z * cos(rad);
	float camY = car.position.y + camLocal.y;

	float lookX = camX + cos(rad) * 300;
	float lookZ = camZ + sin(rad) * 300;
	float lookY = camY;

	gluLookAt(
		camX, camY, camZ,
		lookX, lookY, lookZ,
		0, 1, 0
	);
}

void setupBMWSpotLight(const CarBMW& car) {
	glEnable(GL_LIGHT1);

	GLfloat lightPos[] = {
		car.position.x,
		car.position.y + 300.0f,
		car.position.z,
		1.0f
	};

	GLfloat lightDir[] = { 0.0f, -1.0f, 0.0f };

	GLfloat ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat diffuse[] = { 0.9f,  0.9f,  0.85f, 1.0f };
	GLfloat specular[] = { 1.0f,  1.0f,  1.0f,  1.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDir);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0f);    
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 18.0f);  
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//if (currentCamera == DRIVER)
		//setDriverSeatCamera(bmwCar);
	//else
		camera.Refresh();


	drawGround();

	glCallList(displayListID);
	//setupBMWSpotLight(bmwCar);
	//bmwCar.draw();
	tahoe.draw();
	myElevator.draw();

	sceneLighting.drawStreetLight(Point(-500, -3, 850), 150, 6, 80, 0, 10, true, 1);
	sceneLighting.drawStreetLight(Point(500, -3, 850), 150, 6, 80, 0, 10, false, 2);

	mainDoor.draw();

	glutSwapBuffers();
}

void timer(int value) {
	myElevator.update(camera);

	tahoe.update();

	if (tahoe.isDriving) {
		float angleRad = tahoe.carRotation * (3.14159f / 180.0f);

		float offsetX = 10.0f * cos(angleRad) + 15.0f * sin(angleRad);
		float offsetZ = -20.0f * sin(angleRad) + 15.0f * cos(angleRad);

		float driverX = tahoe.pos.x + offsetX;
		float driverY = tahoe.pos.y + 38.0f; 
		float driverZ = tahoe.pos.z + offsetZ;

		camera.SetPos(tahoe.pos.x + offsetX, tahoe.pos.y + 38.0f, tahoe.pos.z + offsetZ);
		camera.SetYaw(-angleRad);
	}
	else {
		camera.ApplyGravity();
	}

	float cx, cy, cz;
	camera.GetPos(cx, cy, cz);
	mainDoor.update(cx, cz);

	if (tahoe.isDoorOpen && tahoe.doorAngle < 70.0f) {
		tahoe.doorAngle += 2.0f;
	}
	else if (!tahoe.isDoorOpen && tahoe.doorAngle > 0.0f) {
		tahoe.doorAngle -= 2.0f;
	}

	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

void init() {
	glClearColor(0.02f, 0.02f, 0.05f, 1.0f); 

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	displayListID = glGenLists(1);
	glNewList(displayListID, GL_COMPILE);
	mwmShowroom.draw();
	glEndList();

	setupCollision();

	camera.SetPos(0, 20, 1200);
}

void reshape(int w, int h) {
	if (h == 0) h = 1;
	g_iWidth = w; g_iHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / h, g_fNear, g_fFar);
	glMatrixMode(GL_MODELVIEW);
}

static void keyboardCallback(unsigned char key, int x, int y) {
	float step = 15.0f;
	float carAcceleration = 0.2f; 
	float turnSpeed = 3.0f;    
	float cx, cy, cz;
	camera.GetPos(cx, cy, cz);
	float dist = sqrt(pow(cx - tahoe.pos.x, 2) + pow(cz - tahoe.pos.z, 2));

	switch (key) {
	case 27:glutLeaveMainLoop();break;
	case ' ': camera.HandleSpaceTap(); break;

	case 'r': case 'R': {
		if (!tahoe.isDriving) {
			if (dist < 150.0f) { 
				tahoe.isDriving = true;
				tahoe.isDoorOpen = false;
			}
		}
		else {
			tahoe.isDriving = false;
			tahoe.carSpeed = 0;
			camera.SetPos(tahoe.pos.x + 80, 20, tahoe.pos.z);
		}
		break;
	}

	case 'f': case 'F':
		if (dist < 150.0f && !tahoe.isDriving) {
			tahoe.isDoorOpen = !tahoe.isDoorOpen;
		}
		break;

	case 'w': case 'W':
		if (tahoe.isDriving) tahoe.carSpeed += carAcceleration;
		else camera.Move(step);
		break;
	case 's': case 'S':
		if (tahoe.isDriving) tahoe.carSpeed -= carAcceleration;
		else camera.Move(-step);
		break;
	case 'a': case 'A':
		if (tahoe.isDriving) tahoe.carRotation += turnSpeed;
		else camera.Strafe(step);
		break;
	case 'd': case 'D':
		if (tahoe.isDriving) tahoe.carRotation -= turnSpeed;
		else camera.Strafe(-step);
		break;
	case 'q': case 'Q': camera.Fly(step); break;
	case 'e': case 'E': camera.Fly(-step); break;
	case 'o': case 'O': {
		float cx, cy, cz;
		camera.GetPos(cx, cy, cz);

		mwmShowroom.GetBMW().playHorn(Point(cx, cy, cz));
		break;
	}

	case 'l': case 'L':
		myElevator.callElevator(
			camera,
			mwmShowroom.GetElevatorShaftCenter().z
		);
		break;
	}
}

static void specialKeysCallback(int key, int x, int y) {
	float step = 15.0f;
	switch (key) {
	case GLUT_KEY_UP:    camera.Move(step); break;
	case GLUT_KEY_DOWN:  camera.Move(-step); break;
	case GLUT_KEY_LEFT:  camera.Strafe(step); break;
	case GLUT_KEY_RIGHT: camera.Strafe(-step); break;
	}
}

static void mouseMove(int x, int y) {
	if (!g_mouseCaptured) return;

	int dx = x - g_iWidth / 2;
	int dy = y - g_iHeight / 2;

	if (dx != 0 || dy != 0) {
		camera.RotateYaw(dx * g_mouseSensitivity);
		camera.RotatePitch(-dy * g_mouseSensitivity);
		glutWarpPointer(g_iWidth / 2, g_iHeight / 2);
	}
	glutPostRedisplay();
}

static void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		g_mouseCaptured = !g_mouseCaptured; 
		if (g_mouseCaptured) {
			glutSetCursor(GLUT_CURSOR_NONE);
			glutWarpPointer(g_iWidth / 2, g_iHeight / 2);
		}
		else {
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_iWidth, g_iHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("MWM Luxury Car Showroom");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutTimerFunc(16, timer, 0);

	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialKeysCallback);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseButton);

	glutMainLoop();
	return 0;
}