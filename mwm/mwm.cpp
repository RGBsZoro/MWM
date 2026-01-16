#include <Windows.h>
#include <GL/freeglut.h>
#include "Point.h"
#include "Cuboid.h"
#include "Camera.h"

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
Cuboid buildingStructure(Point(0, 0, 0), 100, 630, 300);
Camera camera;
bool g_mouseCaptured = false;
int g_lastMouseX = 0;
int g_lastMouseY = 0;
float g_mouseSensitivity = 0.0025f;


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
	glClearColor(g_background.r, g_background.g, g_background.b, 1.0);
	glLoadIdentity();
	//glTranslatef(0, 0, -10);
	camera.Refresh();

	//setupLighting();
	//setupShadow();
	drawGround();
	glCallList(displayListID);

	glutSwapBuffers();
}


void idle()
{
	display();
}

void timer(int value)
{
	glutTimerFunc(1000 / 30, timer, 0);	//call the timer again each 1 millisecond

}
//initialize some variables
void init()
{
	g_background.r = 255;
	g_background.g = 255;
	g_background.b = 255;

	//load textures here

	//display list
	displayListID = glGenLists(1);
	glNewList(displayListID, GL_COMPILE);
	//glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
	glColor3f(1.0f, 0.0f, 0.0f);
	buildingStructure.draw();
	glEndList();

	// transparency
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


