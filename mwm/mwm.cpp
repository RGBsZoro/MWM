#include <Windows.h>
#include <GL/freeglut.h>
#include "Point.h"
#include "Cuboid.h"
#include "Camera.h"
#include "Door.h"
#include "Lighting.h"
#ifndef GL_LIGHT1
#define GL_LIGHT0 0x4000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#endif
using namespace std;

struct color3f
{
	float r, g, b;
	color3f() { r = 0; g = 0; b = 0; }
	color3f(float r, float g, float b) { this->r = r; this->g = g; this->b = b; }
};

//definition of all functions

void display();
void reshape(int w, int h);
void init();
void timer(int value);
void idle();
void specialKeysUp(int key, int x, int y);
static void keyboardCallback(unsigned char key, int x, int y);
static void specialKeysCallback(int key, int x, int y);
static void mouseMove(int x, int y);
static void mouseButton(int button, int state, int x, int y);




void drawStreet() {
	float startZ = 750.0f; // بداية الرصيف من عند جدار المعرض الأمامي
	float sidewalkW = 100.0f; // عرض الرصيف
	float streetW = 600.0f;   // عرض الشارع
	float totalLength = 3000.0f; // طول الشارع ممتد يميناً ويساراً

	// --- 1. رسم الرصيف (Sidewalk) ---
	// سنستخدم حلقة فور لرسم مربعات الرصيف (أسود وأبيض)
	float pieceLen = 50.0f;
	for (float x = -totalLength / 2; x < totalLength / 2; x += pieceLen) {
		if (((int)(x / pieceLen)) % 2 == 0)
			glColor3ub(240, 240, 240); // أبيض
		else
			glColor3ub(30, 30, 30);    // أسود

		Cuboid tile(Point(x + pieceLen / 2, -1, startZ + sidewalkW / 2), 2, sidewalkW, pieceLen);
		tile.draw();
	}

	// --- 2. رسم الشارع (Asphalt) ---
	glColor3ub(20, 20, 20); // لون أسفلت غامق جداً
	// الشارع يقع بعد الرصيف مباشرة
	Cuboid asphalt(Point(0, -2.5, startZ + sidewalkW + streetW / 2), 1, streetW, totalLength);
	asphalt.draw();

	// --- 3. خطوط الشارع (Street Lines) ---
	glColor3ub(255, 255, 255); // خطوط بيضاء متقطعة
	float dashLen = 60.0f;
	for (float x = -totalLength / 2; x < totalLength / 2; x += dashLen * 2) {
		Cuboid line(Point(x, -2, startZ + sidewalkW + streetW / 2), 1, 10, dashLen);
		line.draw();
	}
}


void drawStrokeText(const char* string, float x, float y, float z, float scale) {
	glPushMatrix();
	glTranslatef(x, y, z); // تحديد مكانه
	glScalef(scale, scale, scale); // تصغير الخط ليتناسب مع المبنى
	glLineWidth(3); // جعل الخط عريضاً وواضحاً

	for (const char* c = string; *c != '\0'; c++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
	glPopMatrix();
}



void drawDetailedBuilding() {
	float totalW = 1500.0f;
	float totalH = 200.0f;
	float totalL = 1500.0f;
	float t = 5.0f;

	// باب كبير لمعرض سيارات (العرض 500 الارتفاع 150)
	float doorW = 500.0f;
	float doorH = 150.0f;

	// 1. الأرضية والسقف والجدران الخارجية
	glColor3f(0.2f, 0.2f, 0.2f); // أرضية المعرض (أسود فخم)
	Cuboid floor(Point(0, -3, 0), t, totalL, totalW); floor.draw();

	glColor3f(0.8f, 0.8f, 0.8f); // سقف
	Cuboid ceiling(Point(0, -3 + totalH, 0), t, totalL, totalW); ceiling.draw();

	glColor3f(0.5f, 0.5f, 0.5f); // جدار خلفي
	Cuboid backWall(Point(0, -3, -totalL / 2), totalH, t, totalW); backWall.draw();

	// جدران جانبية
	glColor3f(0.6f, 0.6f, 0.6f);
	Cuboid rightWall(Point(totalW / 2, -3, 0), totalH, totalL, t); rightWall.draw();
	Cuboid leftWall(Point(-totalW / 2, -3, 0), totalH, totalL, t); leftWall.draw();

	// 2. الجدار الأمامي مع فتحة الباب
	float sideWallW = (totalW - doorW) / 2.0f;
	float sideWallPosX = (totalW / 2.0f) - (sideWallW / 2.0f);

	glColor3f(0.4f, 0.4f, 0.4f);
	Cuboid frontLeft(Point(-sideWallPosX, -3, totalL / 2), totalH, t, sideWallW); frontLeft.draw();
	Cuboid frontRight(Point(sideWallPosX, -3, totalL / 2), totalH, t, sideWallW); frontRight.draw();

	float topPieceH = totalH - doorH;
	Cuboid frontTop(Point(0, -3 + doorH, totalL / 2), topPieceH, t, doorW); frontTop.draw();

	// 3. إضافة اسم المعرض MWM فوق الباب
	// ملاحظة: قمت بتقديم النص قليلاً (totalL/2 + 10) حتى لا يتداخل مع الجدار
	glColor3f(1.0f, 0.9f, 0.0f); // لون ذهبي للنص
	drawStrokeText("MWM", -70, doorH + 9, totalL / 2 + 10, 0.4f);
}




// Global variables
Point center = Point(0, -3, 0);
Texture texture;
Texture up, left, Right, down, front, back;
int g_iWidth = 800;
int g_iHeight = 600;
const float g_fNear = 1;
const float g_fFar = 1000000000.0f;
color3f g_background;
GLuint displayListID;
//Cuboid buildingStructure(Point(0, 0, 0), 100, 630, 300);
Camera camera;
bool g_mouseCaptured = false;
int g_lastMouseX = 0;
int g_lastMouseY = 0;
float g_mouseSensitivity = 0.0025f;
Door mainDoor;
Lighting sceneLighting;


void drawGround()
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.7f, 0.7f, 0.7f);
	glBegin(GL_QUADS);
	glVertex3f(-2000.0f, -3.0f, -2000.0f);
	glVertex3f(2000.0f, -3.0f, -2000.0f);
	glVertex3f(2000.0f, -3.0f, 2000.0f);
	glVertex3f(-2000.0f, -3.0f, 2000.0f);
	glEnd();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_iHeight, g_iWidth);
	glutCreateWindow("weee");
	//glutFullScreen();
	init();
	glutSpecialUpFunc(specialKeysUp);
	glutSpecialFunc(specialKeysCallback);   // For arrow keys
	glutKeyboardFunc(keyboardCallback);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseButton);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	camera.SetPos(700.0f, 10.0f, 50.0f);
	camera.RotateYaw(10.0);


	glutTimerFunc(1, timer, 0);
	glutMainLoop();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// لا نغير إعداداتك الأصلية
	glClearColor(g_background.r, g_background.g, g_background.b, 1.0);
	glLoadIdentity();
	camera.Refresh();

	drawGround();
	drawStreet();
	// رسم الأشياء الثابتة
	glCallList(displayListID);

	// --- الإضافات الاحترافية الجديدة ---

	// العمود الأيسر يستخدم الضوء رقم 1
	sceneLighting.drawStreetLight(Point(-500, -3, 850), 150, 6, 80, 0, 10, true, 1);

	// العمود الأيمن يستخدم الضوء رقم 2 (لاحظ الفرق)
	sceneLighting.drawStreetLight(Point(500, -3, 850), 150, 6, 80, 0, 10, false, 2);


	// 1. رسم الباب المنزلق (شفاف وأوتوماتيكي)
	mainDoor.draw();

	glutSwapBuffers();
}


void idle()
{
	display();
}

void timer(int value)
{
	float cx, cy, cz;
	camera.GetPos(cx, cy, cz); // جلب موقع الكاميرا الحالي

	// تحديث حالة الباب بناءً على موقع الكاميرا
	mainDoor.update(cx, cz);

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}
//initialize some variables
void init()
{
	g_background.r = 0.05;
	g_background.g = 0.05;
	g_background.b = 0.15;

	//load textures here

	//display list
	displayListID = glGenLists(1);
	glNewList(displayListID, GL_COMPILE);
	//glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawDetailedBuilding();
	//buildingStructure.draw();
	glEndList();

	// transparency
	glEnable(GL_DEPTH_TEST); // ضروري جداً لرؤية الأجسام ثلاثية الأبعاد صح
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Optional: if using textures with transparency
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);


}

// function to call when the window is resized
void reshape(int w, int h)
{
	g_iHeight = h;
	g_iWidth = w;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.f, (float)w / (float)h, g_fNear, g_fFar);
	glMatrixMode(GL_MODELVIEW);
}

static void specialKeysCallback(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		camera.Move(10.0);
		break;
	case GLUT_KEY_DOWN:
		camera.Move(-10.0);
		break;
	case GLUT_KEY_LEFT:
		camera.Strafe(10.0);
		break;
	case GLUT_KEY_RIGHT:
		camera.Strafe(-10.0);
		break;
	}
	glutPostRedisplay();
}
static void keyboardCallback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		camera.RotateYaw(-0.02);
		break; // Rotate Left
	case 'd':
		camera.RotateYaw(0.02);
		break; // Rotate Right
	case 'w':
		camera.Fly(2.0);
		break; // Move Up
	case 's':
		camera.Fly(-2.0);
		break; // Move Down
		glutPostRedisplay();
	}
}
void specialKeysUp(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		cout << "up released" << endl;
	if (key == GLUT_KEY_DOWN)
		cout << "down released" << endl;
	if (key == GLUT_KEY_RIGHT)
		cout << "right released" << endl;
	if (key == GLUT_KEY_LEFT)
		cout << "left released" << endl;
}
static void mouseMove(int x, int y)
{
	if (!g_mouseCaptured)
		return;

	int dx = x - g_lastMouseX;
	int dy = y - g_lastMouseY;

	g_lastMouseX = x;
	g_lastMouseY = y;

	camera.RotateYaw(dx * g_mouseSensitivity);
	camera.RotatePitch(-dy * g_mouseSensitivity);

	glutPostRedisplay();
}

static void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		g_mouseCaptured = true;
		g_lastMouseX = x;
		g_lastMouseY = y;

		glutSetCursor(GLUT_CURSOR_NONE);
	}
}


