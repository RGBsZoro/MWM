#include "ShowRoom.h"
#include "Cylinder.h"
#include "Elevator.h"
#include <cmath>

ShowRoom::ShowRoom()
	: bmw(Point(0, 0, 0), "Sounds/car-not-starting.wav")
{
	audiR8.load("models/r8/r8.obj", "models/r8/r8.mtl");
	jaguar.load("models/jaguar/jaguar.obj", "models/jaguar/jaguar.mtl");

}


void ShowRoom::drawJaguar(float x, float y, float z, float rotation) {
	glPushMatrix();

	glTranslatef(x, y + 10.0f, z);
	glRotatef(rotation, 0, 1, 0);

	glScalef(0.04f, 0.04f, 0.04f);

	jaguar.draw();

	glPopMatrix();
}

void ShowRoom::drawSimpleCar(Point p, float scale,
	float r, float g, float b) {
	glPushMatrix();
	glTranslatef(p.x, p.y + 8.0f, p.z);
	glScalef(scale, scale, scale);

	GLfloat bodyMat[] = { r, g, b, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, bodyMat);

	Cuboid(Point(0, 8, 0), 16, 80, 40).draw();

	GLfloat roofMat[] = { r * 0.9f, g * 0.9f, b * 0.9f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, roofMat);

	Cuboid(Point(0, 20, 0), 10, 45, 30).draw();

	GLfloat wheelMat[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, wheelMat);

	Cylinder wheel;
	wheel.draw(Point(-28, 4, 18), 6, 6, 6, 20, 1);
	wheel.draw(Point(28, 4, 18), 6, 6, 6, 20, 1);
	wheel.draw(Point(-28, 4, -18), 6, 6, 6, 20, 1);
	wheel.draw(Point(28, 4, -18), 6, 6, 6, 20, 1);

	glPopMatrix();
}


void ShowRoom::drawSportsCar(float x, float y, float z, float rotation) {
	glPushMatrix();
	glTranslatef(x, y + 1.0f, z);
	glRotatef(rotation, 0, 1, 0);

	glScalef(0.05f, 0.05f, 0.05f);

	audiR8.draw();

	glPopMatrix();
}
void ShowRoom::setupPodiumLighting(int id, float x, float z) {
	GLfloat lightPos[] = { x, 250.0f, z, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	int lightSource = GL_LIGHT1 + (id % 7);
	glEnable(lightSource);
	glLightfv(lightSource, GL_POSITION, lightPos);
	glLightfv(lightSource, GL_DIFFUSE, diffuse);
	glLightfv(lightSource, GL_SPECULAR, specular);

	GLfloat dir[] = { 0, -1, 0 };
	glLightfv(lightSource, GL_SPOT_DIRECTION, dir);
	glLightf(lightSource, GL_SPOT_CUTOFF, 45.0f);
	glLightf(lightSource, GL_SPOT_EXPONENT, 20.0f);
}

void ShowRoom::drawModernPodium(float cx, float cz, int id, int sectionType) {
	float baseR = 180.0f;
	float tierH = 3.9f;
	Cylinder cycl;

	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 80.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	GLfloat sectionColor[3];
	switch (sectionType) {
	case 0: 
		sectionColor[0] = 1.0f; sectionColor[1] = 0.0f; sectionColor[2] = 0.0f;
		break;
	case 1: 
		sectionColor[0] = 0.0f; sectionColor[1] = 0.4f; sectionColor[2] = 0.8f;
		break;
	case 2: 
		sectionColor[0] = 0.0f; sectionColor[1] = 1.0f; sectionColor[2] = 0.2f;
		break;
	case 3:
		sectionColor[0] = 1.0f; sectionColor[1] = 1.0f; sectionColor[2] = 0.0f;
		break;
	default:
		sectionColor[0] = 0.2f; sectionColor[1] = 0.6f; sectionColor[2] = 1.0f;
		break;
	}

	float ringLocalY = 5.0f;
	float ringRadius = baseR - 50.0f;

	glDisable(GL_LIGHTING);
	glColor3fv(sectionColor);
	cycl.draw(
		Point(cx, ringLocalY, cz),
		ringRadius,
		ringRadius,
		0.25f,
		40,
		5
	);
	glEnable(GL_LIGHTING);


	for (int i = 0; i < 3; i++) {
		float r = baseR - (i * 30.0f);
		float y = -3.0f + (i * tierH);

		glDisable(GL_LIGHTING);
		glColor3fv(sectionColor);
		cycl.draw(
			Point(cx, ringLocalY, cz),
			ringRadius,
			ringRadius,
			0.35f,
			40,
			5
		);
		glEnable(GL_LIGHTING);

		glColor3f(0.12f, 0.12f, 0.12f);
		cycl.draw(
			Point(cx, y + tierH, cz),
			r,
			r,
			tierH,
			40,
			5
		);
	}

	setupPodiumLighting(id, cx, cz);
}
void ShowRoom::drawPodiums(float yOffset) {
	float sideX = 400.0f;
	float spacingZ = 450.0f;
	int baseID = (yOffset > 0) ? 6 : 0;

	for (int row = 0; row < 3; row++) {
		float posZ = -450.0f + (row * spacingZ);

		if (yOffset == 0) {
			drawModernPodium(sideX, posZ, baseID + row, 1);
			if (row == 0) {
				drawJaguar(sideX, 0.0f, posZ, -90.0f);
			}

			drawModernPodium(-sideX, posZ, baseID + row + 3, 0);

			if (row == 0) {
				drawSportsCar(-sideX, 10.0f, posZ, 90.0f);
			}

			if (row == 1) {
				glPushMatrix();
				glTranslatef(-sideX, 10.0f, posZ);
				glRotatef(0, 0, 1, 0);
				//glScalef(0.9f, 0.9f, 0.9f);
				bmw.draw();
				glPopMatrix();
			}
			if (row == 2) {
				drawSimpleCar(
					Point(sideX, 0.0f, posZ),
					1.0f,
					0.1f, 0.5f, 0.2f   
				);
			}

			if (row == 1 && yOffset > 0) {
				drawSimpleCar(
					Point(-sideX, yOffset, posZ),
					1.1f,
					0.6f, 0.1f, 0.1f  
				);
			}

			if (row == 2 && yOffset > 0) {
				drawSimpleCar(
					Point(sideX, yOffset, posZ),
					0.95f,
					0.6f, 0.6f, 0.6f   
				);
			}

		}


		else {
			drawModernPodium(sideX, posZ, baseID + row, 2);      
			drawModernPodium(-sideX, posZ, baseID + row + 3, 3);  
		}

		drawCeilingLightRing(Point(sideX, 290, posZ));
		drawCeilingLightRing(Point(-sideX, 290, posZ));
	}
}

CarBMW& ShowRoom::GetBMW() {
	return bmw;
}

void ShowRoom::drawCeilingLightRing(Point p) {
	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
	glLineWidth(6);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 40; i++) {
		float angle = i * 9.0f * 3.14159f / 180.0f;
		glVertex3f(cos(angle) * 160.0f, 0, sin(angle) * 160.0f);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void ShowRoom::drawReceptionDesk() {
	glPushMatrix();

	float yBase = -27.0f; 

	glTranslatef(0, yBase, +totalL / 2 - 180);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

	float bodyHeight = 30.0f;
	float topHeight = 60.0f;
	float seatHeight = 25.0f;
	float backrestHeight = 65.0f;

	glColor3ub(90, 50, 20); 
	Cuboid body(Point(0, bodyHeight, 0), 60, 15, 200);
	body.draw();

	glColor3ub(110, 70, 40);
	Cuboid top(Point(0, topHeight, 0), 5, 50, 220);
	top.draw();

	glColor3ub(20, 20, 20);
	Cuboid seat(Point(0, seatHeight, -60), 10, 40, 40);
	seat.draw();

	Cuboid backrest(Point(0, backrestHeight, -75), 50, 5, 40);
	backrest.draw();

	glPopMatrix();
}

void ShowRoom::drawGlassWallBehindDesk() {
	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float wallHeight = floorHeight;
	float wallThickness = 5.0f;
	float wallWidth = totalW;



	//glTranslatef(0, yPos, zPos);
	glTranslatef(0, -150.0f, +totalL / 2);
	glRotatef(180.0f, 0, 1, 0);

	glColor4f(0.4f, 0.7f, 1.0f, 0.35f);
	Cuboid glass(
		Point(0, wallHeight / 2, 0),
		wallHeight,
		wallThickness,
		wallWidth
	);
	glass.draw();

	
	glDisable(GL_BLEND);

	glPopMatrix();
}
void ShowRoom::drawFloorContent(float yOffset) {
	glPushMatrix();
	glTranslatef(0, yOffset, 0);

	drawPodiums(yOffset);

	if (yOffset > 0) {
		drawReceptionDesk();
		drawGlassWallBehindDesk();
	}


	glPopMatrix();
}

void ShowRoom::drawMWMName() {
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.84f, 0.0f);
	glTranslatef(-85, 165, 755);
	glScalef(0.6f, 0.6f, 0.6f);
	glLineWidth(5);
	const char* text = "MWM";
	for (const char* c = text; *c != '\0'; c++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void ShowRoom::drawBuildingBase() {
	float t = 5.0f;
	float totalBuildingH = floorHeight * 2; 

	glColor3f(0.05f, 0.05f, 0.05f);
	Cuboid(Point(0, -3, 0), t, totalL, totalW).draw();

	glColor3f(0.08f, 0.08f, 0.08f);
	Cuboid(
		Point(0, floorHeight - 3, 0),
		t,
		totalL - 200,   
		totalW
	).draw();

	glColor3f(0.1f, 0.1f, 0.1f);
	Cuboid(Point(0, totalBuildingH - 3, 0), t, totalL, totalW).draw();

	glColor3f(0.3f, 0.3f, 0.3f);

	Cuboid(
		Point(-totalW / 2 + (totalW - 120) / 4, -3, -totalL / 2),
		totalBuildingH,
		5,
		(totalW - 120) / 2
	).draw();

	Cuboid(
		Point(totalW / 2 - (totalW - 120) / 4, -3, -totalL / 2),
		totalBuildingH,
		5,
		(totalW - 120) / 2
	).draw();

	Cuboid(Point(totalW / 2, -3, 0), totalBuildingH, totalL, t).draw();   
	Cuboid(Point(-totalW / 2, -3, 0), totalBuildingH, totalL, t).draw();  

	Cuboid(
		Point(0, 100, -totalL / 2),
		floorHeight - 100,
		5,
		120
	).draw();

	Cuboid(
		Point(0, floorHeight + 100, -totalL / 2),
		floorHeight - 100,
		5,
		120
	).draw();

	float sideWallW = (totalW - doorW) / 2.0f;
	float posX = (totalW / 2.0f) - (sideWallW / 2.0f);

	Cuboid(
		Point(-posX, -3, totalL / 2),
		floorHeight,
		t,
		sideWallW
	).draw();

	Cuboid(
		Point(posX, -3, totalL / 2),
		floorHeight,
		t,
		sideWallW
	).draw();

	Cuboid(
		Point(0, doorH - 3, totalL / 2),
		floorHeight - doorH,
		t,
		doorW
	).draw();
}


void ShowRoom::drawTree(float x, float z) {
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


void ShowRoom::drawOppositeSideline() {
	float treeZ = 1550.0f;
	float totalLength = 4000.0f;

	glColor3ub(50, 50, 50); 
	Cuboid curb(Point(0, -1, treeZ), 2, 80, totalLength);
	curb.draw();

	float startX = -totalLength / 2;
	float step = 300.0f; 

	for (float x = startX + 100; x < totalLength / 2; x += step) {
		drawTree(x, treeZ);
	}
}

void ShowRoom::drawStreet() {
	float startZ = 750.0f;
	float pieceLen = 50.0f;
	float streetWidth = 700.0f;
	float streetLength = 4000.0f;

	for (float x = -2000; x < 2000; x += pieceLen) {
		if (((int)(x / pieceLen)) % 2 == 0) glColor3ub(220, 220, 220);
		else glColor3ub(40, 40, 40);
		Cuboid(Point(x + pieceLen / 2, -1, startZ + 50), 2, 100, pieceLen).draw();
	}

	glColor3ub(15, 15, 15);
	Cuboid(Point(0, -2.5, startZ + 450), 1, streetWidth, streetLength).draw();

	glColor3ub(255, 255, 255);
	float lineLen = 80.0f;
	float gapLen = 120.0f;
	float lineWidth = 8.0f;     

	for (float x = -2000; x < 2000; x += (lineLen + gapLen)) {
		Cuboid(Point(x + lineLen / 2, -0.8, startZ + 450), 0.2f, lineWidth, lineLen).draw();
	}
}

void ShowRoom::draw() {
	drawStreet();
	drawOppositeSideline();
	glEnable(GL_LIGHTING);
	drawBuildingBase();
	drawMWMName();
	drawFloorContent(0);             
	drawFloorContent(floorHeight);    
	glDisable(GL_LIGHTING);
}

// --- 9. التصادمات ---
std::vector<Wall> ShowRoom::GetStaticWalls() {
	std::vector<Wall> w;
	float t = 5.0f;
	float totalBuildingH = 600.0f;
	float shaftW = 120.0f;
	float sideW = (totalW - shaftW) / 2.0f;

	w.push_back(Cuboid(Point(-sideW / 2, -3, -totalL / 2), totalBuildingH, t, sideW).ToWall());
	w.push_back(Cuboid(Point(+sideW / 2, -3, -totalL / 2), totalBuildingH, t, sideW).ToWall());
	w.push_back(Cuboid(Point(totalW / 2, -3, 0), totalBuildingH, totalL, t).ToWall());
	w.push_back(Cuboid(Point(-totalW / 2, -3, 0), totalBuildingH, totalL, t).ToWall());
	w.push_back(
		Cuboid(
			Point(
				0.0f,
				floorHeight - 150.0f,   
				+totalL / 2 - 2.5f      
			),
			floorHeight,               
			5.0f,                      
			totalW - 120.0f            
		).ToWall()
	);

	for (int floor = 0; floor < 2; floor++) {
		float yOff = floor * 300.0f;
		for (int row = 0; row < 3; row++) {
			float pz = -450.0f + (row * 450.0f);
			w.push_back(Cuboid(Point(400.0f, -3 + yOff, pz), 9, 300, 300).ToWall());
			w.push_back(Cuboid(Point(-400.0f, -3 + yOff, pz), 9, 300, 300).ToWall());
		}
	}
	w.push_back(Cuboid(Point(0, 300 - 3, -totalL / 2 + 180), 60, 40, 220).ToWall());
	return w;
}

Wall ShowRoom::GetMainDoorWall() {
	return Cuboid(Point(0, -3, totalL / 2), doorH, 10.0f, doorW).ToWall();
}

Point ShowRoom::GetElevatorShaftCenter() const {
	return Point(0.0f, -3.0f, -totalL / 2 + 2.5f);
}

float ShowRoom::GetElevatorDoorWidth() const { return 120.0f; }
float ShowRoom::GetFloorHeight() const { return floorHeight; }