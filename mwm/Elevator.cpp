#include "Elevator.h"

// ================= Constructor =================
Elevator::Elevator(Point startPos, float targetH)
    : position(startPos),
    bottomY(startPos.y),              // أرضية الطابق الأرضي
    topY(startPos.y + targetH),       // أرضية الطابق العلوي
    currentY(startPos.y),             // يبدأ على الأرض تمامًا
    speed(1.2f),
    cabinW(420.0f),
    cabinD(650.0f),
    cabinH(260.0f),
    doorState(DoorState::CLOSED),
    doorWidth(0.0f),
    maxDoorWidth(380.0f),
    state(ElevatorState::IDLE)
{
}

// ================= Utils =================
bool Elevator::isAtFloor() const {
    return fabs(currentY - bottomY) < 1.0f ||
        fabs(currentY - topY) < 1.0f;
}

// ================= Call Elevator =================
// تعديل دالة callElevator لتكون أكثر دقة عند ضغط L
void Elevator::callElevator(Camera& cam, float targetZ) {
    float cx, cy, cz;
    cam.GetPos(cx, cy, cz);

    // التحقق من أن اللاعب يقف أمام فتحة المصعد (الجدار الخلفي)
    bool nearShaft = fabs(cx - position.x) < 150.0f &&
        fabs(cz - targetZ) < 100.0f;

    if (!nearShaft) return;

    if (doorState == DoorState::OPEN) {
        doorState = DoorState::CLOSING;
    }
    else if (doorState == DoorState::CLOSED && state == ElevatorState::IDLE) {
        // إذا كان المصعد في الطابق الآخر، يتحرك إليك
        // إذا كان في نفس طابقك، يفتح الباب فقط
        float distToBottom = fabs(currentY - bottomY);
        float distToTop = fabs(currentY - topY);
        float playerY = cy;

        if (playerY < (bottomY + topY) / 2) { // اللاعب في الأرضي
            if (distToBottom < 5.0f) doorState = DoorState::OPENING;
            else state = ElevatorState::MOVING_DOWN;
        }
        else { // اللاعب في الأول
            if (distToTop < 5.0f) doorState = DoorState::OPENING;
            else state = ElevatorState::MOVING_UP;
        }
    }
}
// ================= Update =================
void Elevator::update(Camera& cam) {

    // ===== الأبواب =====
    if (doorState == DoorState::OPENING) {
        doorWidth += 3.0f;
        if (doorWidth >= maxDoorWidth) {
            doorWidth = maxDoorWidth;
            doorState = DoorState::OPEN;
        }
    }
    else if (doorState == DoorState::CLOSING) {
        doorWidth -= 3.0f;
        if (doorWidth <= 0.0f) {
            doorWidth = 0.0f;
            doorState = DoorState::CLOSED;
        }
    }

    // ===== الحركة =====
    if (state != ElevatorState::IDLE && doorState == DoorState::CLOSED) {

        float oldY = currentY;

        if (state == ElevatorState::MOVING_UP) {
            currentY += speed;
            if (currentY >= topY) {
                currentY = topY;
                state = ElevatorState::IDLE;
                doorState = DoorState::OPENING;
            }
        }
        else if (state == ElevatorState::MOVING_DOWN) {
            currentY -= speed;
            if (currentY <= bottomY) {
                currentY = bottomY;
                state = ElevatorState::IDLE;
                doorState = DoorState::OPENING;
            }
        }

        // ربط اللاعب بأرضية المصعد
        if (isPlayerInside(cam)) {
            cam.SetPos(cam.m_x,
                cam.m_y + (currentY - oldY),
                cam.m_z);
        }
    }
}

// ================= Collision =================
bool Elevator::isPlayerInside(Camera& cam) {
    float cx, cy, cz;
    cam.GetPos(cx, cy, cz);

    bool insideX = fabs(cx - position.x) <= cabinW / 2;
    bool insideZ = (cz <= position.z + 5) &&
        (cz >= position.z - cabinD);

    // اللاعب يقف على أرضية المصعد
    bool insideY = (cy >= currentY) &&
        (cy <= currentY + cabinH);

    return insideX && insideZ && insideY;
}
// ================= Drawing Helpers =================
void Elevator::drawCeilingLight() {
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 0.95f, 0.9f);

    glBegin(GL_QUADS);
    glVertex3f(-cabinW / 4, cabinH - 5, -cabinD / 4);
    glVertex3f(cabinW / 4, cabinH - 5, -cabinD / 4);
    glVertex3f(cabinW / 4, cabinH - 5, cabinD / 4);
    glVertex3f(-cabinW / 4, cabinH - 5, cabinD / 4);
    glEnd();

    glEnable(GL_LIGHTING);
}

void Elevator::drawControlPanel() {
    glPushMatrix();
    glTranslatef(cabinW / 2 - 15, 120, -cabinD / 2 + 30);

    glColor3f(0.05f, 0.05f, 0.05f);
    Cuboid(Point(0, 0, 0), 140, 8, 30).draw();

    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 0.0f);
    Cuboid(Point(0, 40, 6), 12, 4, 12).draw();

    glColor3f(1.0f, 0.0f, 0.0f);
    Cuboid(Point(0, -40, 6), 12, 4, 12).draw();
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void Elevator::drawFloorIndicator() {
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 0.8f);

    glPushMatrix();
    glTranslatef(-30, cabinH - 40, cabinD / 2 + 2);
    glScalef(0.2f, 0.2f, 0.2f);

    const char* txt = (fabs(currentY - bottomY) < 10) ? "G" : "1";
    for (const char* c = txt; *c; c++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// ================= Draw =================
// في Elevator.cpp - تعديل دالة Draw
void Elevator::draw() {

    // خط Debug عند أرضية المصعد
    /*glDisable(GL_LIGHTING);
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(-100, 0, 0);
    glVertex3f(100, 0, 0);
    glEnd();
    glEnable(GL_LIGHTING);*/

    glPushMatrix();

    // تحريك المصعد بحيث يكون Y = أرضية المصعد
    glTranslatef(position.x, currentY, position.z);

    // ===== الكبينة =====
    // مركز الكبينة على ارتفاع cabinH/2 فوق الأرض
    glColor3f(0.15f, 0.15f, 0.15f);
    Cuboid(
        Point(0, cabinH / 2, -cabinD / 2),
        cabinH, cabinD, cabinW
    ).draw();

    // ===== الأبواب =====
    float doorH = 220.0f;
    glColor3f(0.3f, 0.3f, 0.3f);

    Cuboid(
        Point(-(maxDoorWidth / 2 + doorWidth) / 2, doorH / 2, 0),
        doorH, 5, maxDoorWidth / 2 - doorWidth / 2
    ).draw();

    Cuboid(
        Point((maxDoorWidth / 2 + doorWidth) / 2, doorH / 2, 0),
        doorH, 5, maxDoorWidth / 2 - doorWidth / 2
    ).draw();

    // ===== الداخل =====
    glPushMatrix();
    glTranslatef(0, 0, -cabinD / 2);
    drawCeilingLight();
    drawControlPanel();
    glPopMatrix();

    glPopMatrix();
}
