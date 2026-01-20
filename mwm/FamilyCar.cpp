#include "FamilyCar.h"
#include "Sofa.h"
#include "Cylinder.h"
void FamilyCar::draw(int r , int g , int b) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(carRotation, 0, 1, 0);

    float scale = 0.75f; 
    float smallRadius = wheelRadius * scale;
    float wheelWidth = width * 0.15f;
    float xDist = length * 0.35f;
    float zDist = width / 2.0f;

    float wPos[4][3] = {
        { xDist, smallRadius,  zDist},
        { xDist, smallRadius, -zDist - wheelWidth},
        {-xDist, smallRadius,  zDist},
        {-xDist, smallRadius, -zDist - wheelWidth}
    };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(wPos[i][0], wPos[i][1], wPos[i][2]);

        wheel.draw(smallRadius, wheelWidth);

        glPopMatrix();
    }

    glColor3ub(30, 30, 30);
    float chassisLen = length * 0.89f;
    Cuboid base(Point(0, wheelRadius + 2.0f, 0), 4.0f, width - 2.0f, chassisLen);
    base.draw();

    drawInterior();


    float doorY = wheelRadius - 2.0f; 
    float doorH = 16.0f;              
    float winH = 22.0f;              
    float roofH = doorY + doorH + winH; 

    float sideDist = width / 2.0f;
    float roofInward = 2.5f;          

    float windshieldStart = length * 0.28f; 
    float bPillarX = -length * 0.05f;       
    float rearWindowStart = -length * 0.45f;
    float backDoorEnd = -xDist + 2.0f;

    float trunkEnd = -length * 0.55f;
    float trunkH = doorY + doorH + 7.0f; 
    glPushMatrix();
    glColor3ub(40, 40, 40);
    float pillarW = 2.0f; 

    glBegin(GL_QUADS);
    glVertex3f(bPillarX + pillarW, doorY + 4, sideDist - 0.5f);
    glVertex3f(bPillarX - pillarW, doorY + 4, sideDist - 0.5f);
    glVertex3f(bPillarX - pillarW, roofH, sideDist - roofInward);
    glVertex3f(bPillarX + pillarW, roofH, sideDist - roofInward);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(bPillarX + pillarW, doorY + 4, -sideDist + 0.5f);
    glVertex3f(bPillarX - pillarW, doorY + 4, -sideDist + 0.5f);
    glVertex3f(bPillarX - pillarW, roofH, -sideDist + roofInward);
    glVertex3f(bPillarX + pillarW, roofH, -sideDist + roofInward);
    glEnd();

    glColor3ub(r,g,b);
    glBegin(GL_QUADS);
    glVertex3f(windshieldStart, roofH, sideDist - roofInward);
    glVertex3f(windshieldStart, roofH, -sideDist + roofInward);
    glVertex3f(rearWindowStart, roofH, -sideDist + roofInward);
    glVertex3f(rearWindowStart, roofH, sideDist - roofInward);
    glEnd();
    glPopMatrix();


    float doorThick = 1.5f;
    float frontDoorLen = (windshieldStart - (bPillarX + pillarW));
    glPushMatrix();
    glTranslatef(windshieldStart, doorY, sideDist);
    glRotatef(doorAngle, 0, 1, 0);
    glColor3ub(r,g,b);
    Cuboid dDoor(Point(-frontDoorLen / 2.0f, doorH / 2.0f, 0), doorH, doorThick, frontDoorLen);
    dDoor.draw();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.8f, 0.9f, 1.0f, 0.4f);
    glBegin(GL_QUADS);
    glVertex3f(0, doorH, 0);                 
    glVertex3f(-frontDoorLen, doorH, 0);     
    glVertex3f(-frontDoorLen, doorH + winH, -roofInward); 
    glVertex3f(0, doorH + winH, -roofInward); 
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(windshieldStart, doorY, -sideDist);
    glColor3ub(r,g,b);
    Cuboid pDoor(Point(-frontDoorLen / 2.0f, doorH / 2.0f, 0), doorH, doorThick, frontDoorLen);
    pDoor.draw();

    glEnable(GL_BLEND);
    glColor4f(0.8f, 0.9f, 1.0f, 0.4f);
    glBegin(GL_QUADS);
    glVertex3f(0, doorH, 0);
    glVertex3f(-frontDoorLen, doorH, 0);
    glVertex3f(-frontDoorLen, doorH + winH, roofInward);
    glVertex3f(0, doorH + winH, roofInward);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();

    float backDoorStart = bPillarX - pillarW;
    float backDoorLen = (backDoorStart - rearWindowStart);

    glPushMatrix();
    glTranslatef(backDoorStart - backDoorLen / 2.0f, doorY + doorH / 2.0f, sideDist);
    glColor3ub(r,g,b);
    Cuboid rlDoor(Point(0, 0, 0), doorH, doorThick, backDoorLen);
    rlDoor.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(backDoorStart, doorY, sideDist);
    glEnable(GL_BLEND);
    glColor4f(0.8f, 0.9f, 1.0f, 0.4f);
    glBegin(GL_QUADS);
    glVertex3f(0, doorH, 0);
    glVertex3f(-backDoorLen, doorH, 0);
    glVertex3f(-backDoorLen, doorH + winH, -roofInward);
    glVertex3f(0, doorH + winH, -roofInward);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(backDoorStart - backDoorLen / 2.0f, doorY + doorH / 2.0f, -sideDist);
    glColor3ub(r,g,b);
    Cuboid rrDoor(Point(0, 0, 0), doorH, doorThick, backDoorLen);
    rrDoor.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(backDoorStart, doorY, -sideDist);
    glEnable(GL_BLEND);
    glColor4f(0.8f, 0.9f, 1.0f, 0.4f);
    glBegin(GL_QUADS);
    glVertex3f(0, doorH, 0);
    glVertex3f(-backDoorLen, doorH, 0);
    glVertex3f(-backDoorLen, doorH + winH, roofInward);
    glVertex3f(0, doorH + winH, roofInward);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();

   

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.7f, 0.85f, 1.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(windshieldStart + 15.0f, doorY + doorH + 9, sideDist); 
    glVertex3f(windshieldStart + 15.0f, doorY + doorH + 9, -sideDist);
    glVertex3f(windshieldStart, roofH, -sideDist + roofInward); 
    glVertex3f(windshieldStart, roofH, sideDist - roofInward);
    glEnd();
 

    glBegin(GL_QUADS);
    glVertex3f(rearWindowStart, roofH, sideDist - roofInward);  
    glVertex3f(rearWindowStart, roofH, -sideDist + roofInward); 
    glVertex3f(rearWindowStart, trunkH, -sideDist);             
    glVertex3f(rearWindowStart, trunkH, sideDist);              
    glEnd();
    glDisable(GL_BLEND);

    float hoodBackX = windshieldStart + 15.0f; 
    float hoodFrontX = length * 0.58f;        
    
    float hoodHighY = doorY + doorH + 9.5f;   
    float hoodLowY = doorY + doorH + 2.0f;    

    glPushMatrix();
    glColor3ub(r,g,b);

    glBegin(GL_POLYGON);
    glVertex3f(hoodFrontX, hoodLowY, sideDist);
    glVertex3f(hoodFrontX, hoodLowY, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, sideDist);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(hoodBackX, hoodHighY, sideDist);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(hoodBackX, doorY + doorH, -sideDist);
    glVertex3f(hoodBackX, doorY + doorH, sideDist);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(hoodFrontX, hoodLowY, sideDist);
    glVertex3f(hoodFrontX, hoodLowY, -sideDist);
    glVertex3f(hoodFrontX, doorY + 1.0f, -sideDist); 
    glVertex3f(hoodFrontX, doorY + 1.0f, sideDist);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(hoodBackX, hoodHighY, sideDist);
    glVertex3f(hoodFrontX, hoodLowY, sideDist);
    glVertex3f(hoodFrontX, doorY + 1.0f, sideDist);
    glVertex3f(hoodBackX, doorY + 1.0f, sideDist);

    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(hoodFrontX, hoodLowY, -sideDist);
    glVertex3f(hoodFrontX, doorY + 1.0f, -sideDist);
    glVertex3f(hoodBackX, doorY + 1.0f, -sideDist);
    glEnd();

    glPopMatrix();


    glPushMatrix();
    glColor3ub(r,g,b); 
    glBegin(GL_POLYGON);
    glVertex3f(windshieldStart, doorY + doorH - 8.0f, sideDist); 
    glVertex3f(hoodBackX, doorY + doorH - 8.0f, sideDist);      
    glVertex3f(hoodBackX, hoodHighY, sideDist);                 
    glVertex3f(windshieldStart, roofH - winH, sideDist);        
    glVertex3f(windshieldStart - 2.0f, doorY + doorH, sideDist); 
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(windshieldStart, doorY + doorH - 8.0f, -sideDist);
    glVertex3f(hoodBackX, doorY + doorH - 8.0f, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(windshieldStart, roofH - winH, -sideDist);
    glVertex3f(windshieldStart - 2.0f, doorY + doorH, -sideDist);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glColor3ub(r,g,b);

    glBegin(GL_QUADS);
    glVertex3f(windshieldStart, roofH, sideDist - roofInward);   
    glVertex3f(hoodBackX, hoodHighY, sideDist);                  
    glVertex3f(windshieldStart, hoodHighY, sideDist);           
    glVertex3f(windshieldStart, roofH - winH, sideDist);         

    glVertex3f(windshieldStart, roofH, -sideDist + roofInward);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(windshieldStart, hoodHighY, -sideDist);
    glVertex3f(windshieldStart, roofH - winH, -sideDist);
    glEnd();

    glPopMatrix();

 
    glPushMatrix();
    glColor3ub(r,g,b);
    glBegin(GL_POLYGON); 
    glVertex3f(hoodFrontX, hoodLowY, sideDist);
    glVertex3f(hoodFrontX, hoodLowY, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, sideDist);
    glEnd();
    
    glPopMatrix();

    glBegin(GL_QUADS);
    glColor3ub(r,g,b); 
    glVertex3f(rearWindowStart, trunkH, sideDist);  
    glVertex3f(rearWindowStart, trunkH, -sideDist); 
    glVertex3f(rearWindowStart, doorY + 3, -sideDist);  
    glVertex3f(rearWindowStart, doorY + 3, sideDist);   
    glEnd();


    glPushMatrix();
    if (headlightsOn) {
        glColor3ub(255, 255, 200);
    }
    else {
        glColor3ub(50, 50, 40);   
    }    float lightSize = 3.0f;
    float lightY = doorY + 6.0f;

    glBegin(GL_QUADS);
    glVertex3f(hoodFrontX + 0.1f, lightY + lightSize, sideDist - 2.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY - lightSize, sideDist - 2.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY - lightSize, sideDist - 7.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY + lightSize, sideDist - 7.0f);

    glVertex3f(hoodFrontX + 0.1f, lightY + lightSize, -sideDist + 2.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY - lightSize, -sideDist + 2.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY - lightSize, -sideDist + 7.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY + lightSize, -sideDist + 7.0f);
    glEnd();
    glPopMatrix();






    glPushMatrix();
    glColor3ub(200, 0, 0); 
    float tailLightY = doorY + 8.0f;
    glBegin(GL_QUADS);
    glVertex3f(rearWindowStart - 0.1f, tailLightY + 2.5f, sideDist - 1.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY - 2.5f, sideDist - 1.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY - 2.5f, sideDist - 6.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY + 2.5f, sideDist - 6.0f);

    glVertex3f(rearWindowStart - 0.1f, tailLightY + 2.5f, -sideDist + 1.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY - 2.5f, -sideDist + 1.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY - 2.5f, -sideDist + 6.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY + 2.5f, -sideDist + 6.0f);
    glEnd();
    glPopMatrix();



    glPushMatrix();
    glColor3ub(40, 40, 40); 

    glPushMatrix();
    glTranslatef(windshieldStart + 2.0f, doorY + doorH + 1.0f, sideDist + 0.5f);
    Cuboid mirrorR(Point(0, 8.0f, 1.0f), 2.0f, 3.0f, 1.2f); 
    mirrorR.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(windshieldStart + 2.0f, doorY + doorH + 1.0f, -sideDist - 0.5f);
    Cuboid mirrorL(Point(0, 8.0f, -1.0f), 2.0f, 3.0f, 1.2f);
    mirrorL.draw();
    glPopMatrix();

    glPopMatrix();


    glPopMatrix();
}




void FamilyCar::drawInterior() {
    Cylinder cylinderTool;

    float seatY = wheelRadius + 5.0f;
    float fSeatX = length * 0.1f;
    float fSeatW = 18.0f;
    float fBaseL = 18.0f;
    float commonBackH = 22.0f;

    auto drawOneSeat = [&](float zPos) {
        glPushMatrix();
        glTranslatef(fSeatX, seatY, zPos);
        glColor3ub(40, 40, 40);
        Cuboid base(Point(0, 1.5f, 0), 6.0f, fSeatW, fBaseL);
        base.draw();

        glPushMatrix();
        glTranslatef(-fBaseL / 2.0f + 2.0f, 1.5f, 0);
        glRotatef(15, 0, 0, 1);
        glColor3ub(50, 50, 50);
        Cuboid back(Point(0, commonBackH / 8.0f, 0), commonBackH, fSeatW, 4.0f);
        back.draw();
        glPopMatrix();
        glPopMatrix();
        };

    drawOneSeat(width * 0.25f);
    drawOneSeat(-width * 0.25f);

    glPushMatrix();
    float backSeatX = -length * 0.15f;
    glTranslatef(backSeatX, seatY, 0);
    glColor3ub(30, 30, 30);
    float backSeatW = width - 6.0f;
    float backBaseL = 18.0f;
    Cuboid rearBase(Point(0, 1.5f, 0), 6.0f, backSeatW, backBaseL);
    rearBase.draw();

    glPushMatrix();
    glTranslatef(-backBaseL / 2.0f + 2.0f, 1.5f, 0);
    glRotatef(12, 0, 0, 1);
    glColor3ub(40, 40, 40);
    Cuboid rearBack(Point(0, commonBackH / 8.0f, 0), commonBackH, backSeatW, 4.0f);
    rearBack.draw();
    glPopMatrix();
    glPopMatrix();


    float dashX = length * 0.35f;
    float dashH = 14.0f;
    float dashW = width - 4.0f;

    glPushMatrix();
    glTranslatef(dashX, seatY + 4.0f, 0);
    glRotatef(-15, 0, 0, 1);
    glColor3ub(20, 20, 20);
    Cuboid dashboard(Point(0, 0, 0), dashH, dashW, 12.0f);
    dashboard.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(dashX - 15.0f, seatY + 12.0f, width * 0.25f);

    glRotatef(90, 0, 1, 0);
    glRotatef(20, 1, 0, 0);

    glColor3ub(10, 10, 10);
    glutSolidTorus(1.0, 4.5, 15, 30);

    glColor3ub(30, 30, 30);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    cylinderTool.draw(Point(0, 0.6, 0), 1.0f, 2.5f, 15.0f, 15, 10);
    glPopMatrix();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(dashX - 1.0f, seatY + 8.0f, width * 0.25f);
    glRotatef(-15, 0, 0, 1);
    glColor3ub(50, 50, 60);
    Cuboid cluster(Point(0, 0, 0), 6.0f, 14.0f, 2.0f);
    cluster.draw();
    glTranslatef(-0.5f, 0, 0);
    glColor3ub(0, 191, 255);
    Cuboid screen(Point(0, 0, 0), 4.0f, 12.0f, 1.0f);
    screen.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(fSeatX + 8.0f, seatY, 0);
    glColor3ub(25, 25, 25);
    Cuboid console(Point(0, 1.0f, 0), 4.0f, 8.0f, 15.0f);
    console.draw();

    glTranslatef(4.0f, 3.5f, 0);

    glPushMatrix();
    glColor3ub(180, 180, 180);
    glRotatef(0, 1, 0, 0);
    cylinderTool.draw(Point(0, 5, 0), 0.5f, 0.5f, 6.0f, 10, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 6.0f, 0);
    glColor3ub(10, 10, 10);
    glutSolidSphere(1.8, 15, 15);
    glPopMatrix();

    glPopMatrix();
}
