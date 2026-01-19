#include <Windows.h>
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
Camera camera;
Door mainDoor;
Lighting sceneLighting;
ShowRoom mwmShowroom;
// تعديل السطر في main.cpp
// وضعناه عند z = -315 ليكون خلف الجدار الخلفي مباشرة
// والارتفاع يبدأ من -3.0f لينطبق مع الأرضية
Elevator myElevator(
	mwmShowroom.GetElevatorShaftCenter(),
	mwmShowroom.GetFloorHeight()
);
GLuint displayListID;
bool g_mouseCaptured = false;
int g_lastMouseX = 400;
int g_lastMouseY = 300;
float g_mouseSensitivity = 0.0015f; // حساسية أنعم قليلاً

int g_iWidth = 1024;
int g_iHeight = 768;
const float g_fNear = 1.0f;
const float g_fFar = 15000.0f;

// التصادم
void setupCollision() {
	camera.walls.clear();
	camera.doorWalls.clear();
	camera.Doors.clear();

	// جلب الجدران والمنصات من المعرض
	camera.walls = mwmShowroom.GetStaticWalls();

	// ربط الباب الرئيسي
	camera.Doors.push_back(&mainDoor);
	camera.doorWalls.push_back(mwmShowroom.GetMainDoorWall());
}

void drawGround() {
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.1f, 0.1f, 0.1f); // أرضية خارجية داكنة للشارع
	glBegin(GL_QUADS);
	glVertex3f(-5000.0f, -3.1f, -5000.0f);
	glVertex3f(5000.0f, -3.1f, -5000.0f);
	glVertex3f(5000.0f, -3.1f, 5000.0f);
	glVertex3f(-5000.0f, -3.1f, 5000.0f);
	glEnd();
	glEnable(GL_LIGHTING);
}

void drawTree(float x, float z) {
	// 1. جذع الشجرة (بني)
	glColor3ub(101, 67, 33);
	Cuboid trunk(Point(x, 0.0f, z), 50.0f, 12.0f, 12.0f);
	trunk.draw();

	// 2. أوراق الشجرة (أخضر متدرج)
	// الطبقة السفلية الكبيرة
	glColor3ub(34, 139, 34);
	Cuboid leaves1(Point(x, 45.0f, z), 40.0f, 70.0f, 70.0f);
	leaves1.draw();

	// الطبقة الوسطى
	glColor3ub(46, 170, 46);
	Cuboid leaves2(Point(x, 75.0f, z), 35.0f, 55.0f, 55.0f);
	leaves2.draw();

	// الطبقة العلوية (القمة)
	glColor3ub(60, 200, 60);
	Cuboid leaves3(Point(x, 100.0f, z), 25.0f, 35.0f, 35.0f);
	leaves3.draw();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	camera.Refresh();
	drawGround();

	// رسم محتويات المعرض الثابتة
	glCallList(displayListID);
	myElevator.draw();

	// إضاءة الشوارع (تحتاج تحديث مستمر للإضاءة)
	sceneLighting.drawStreetLight(Point(-500, -3, 850), 150, 6, 80, 0, 10, true, 1);
	sceneLighting.drawStreetLight(Point(500, -3, 850), 150, 6, 80, 0, 10, false, 2);

	// رسم الباب المتحرك
	mainDoor.draw();

	glutSwapBuffers();
}

void timer(int value) {
	myElevator.update(camera); // تحديث موقع المصعد والكاميرا معاً
	camera.ApplyGravity();

	float cx, cy, cz;
	camera.GetPos(cx, cy, cz);
	mainDoor.update(cx, cz); // فتح الباب عند اقتراب الكاميرا

	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

void init() {
	glClearColor(0.02f, 0.02f, 0.05f, 1.0f); // سماء ليلية غامقة جداً

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	// إعداد إضاءة خافتة عامة (Ambient)
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	// إنشاء الـ Display List
	displayListID = glGenLists(1);
	glNewList(displayListID, GL_COMPILE);
	mwmShowroom.draw();
	glEndList();

	setupCollision();

	// وضع الكاميرا في الخارج لمشاهدة المعرض من بعيد عند البدء
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
	switch (key) {
	case 27: exit(0); break;
	case ' ': camera.HandleSpaceTap(); break;
	case 'w': case 'W': camera.Move(step); break;
	case 's': case 'S': camera.Move(-step); break;
	case 'a': case 'A': camera.Strafe(step); break;
	case 'd': case 'D': camera.Strafe(-step); break;
	case 'q': case 'Q': camera.Fly(step); break;
	case 'e': case 'E': camera.Fly(-step); break;
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
		// إعادة الماوس للمركز لمنع خروجه من الشاشة
		glutWarpPointer(g_iWidth / 2, g_iHeight / 2);
	}
	glutPostRedisplay();
}

static void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		g_mouseCaptured = !g_mouseCaptured; // تبديل وضع الماوس (Lock/Unlock)
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