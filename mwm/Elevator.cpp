#include "Elevator.h"
Elevator::Elevator(Point startPos, float targetH)
    : position(startPos),
    bottomY(startPos.y),
    topY(startPos.y + targetH),
    currentY(startPos.y),
    speed(1.5f),
    doorState(DoorState::CLOSED),
    doorWidth(0.0f),
    maxDoorWidth(120.0f),
    state(ElevatorState::IDLE)
{
}
bool Elevator::isAtFloor() const {
    return fabs(currentY - bottomY) < 1.0f ||
        fabs(currentY - topY) < 1.0f;
}
void Elevator::callElevator(Camera& cam, float backWallZ) {
    float cx, cy, cz;
    cam.GetPos(cx, cy, cz);

    if (cz < backWallZ + 150 && cz > backWallZ - 50) {
        if (doorState == DoorState::OPEN) {
            doorState = DoorState::CLOSING;
        }
        else if (doorState == DoorState::CLOSED && state == ElevatorState::IDLE) {
            if (fabs(currentY - bottomY) < 5.0f)
                state = ElevatorState::MOVING_UP;
            else
                state = ElevatorState::MOVING_DOWN;
        }
    }
}
void Elevator::update(Camera& cam) {

    // الأبواب
    if (doorState == DoorState::OPENING) {
        doorWidth += 1.0f;
        if (doorWidth >= maxDoorWidth)
            doorState = DoorState::OPEN;
    }
    else if (doorState == DoorState::CLOSING) {
        doorWidth -= 1.0f;
        if (doorWidth <= 0.0f) {
            doorWidth = 0.0f;
            doorState = DoorState::CLOSED;
        }
    }

    // حركة المصعد
    if (doorState == DoorState::CLOSED) {
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

        // ربط اللاعب بالمصعد
        if (isPlayerInside(cam)) {
            cam.SetPos(cam.m_x, cam.m_y + (currentY - oldY), cam.m_z);
        }
    }
}

bool Elevator::isPlayerInside(Camera& cam) {
    float cx, cy, cz;
    cam.GetPos(cx, cy, cz);

    float halfW = maxDoorWidth / 2.0f;

    bool insideX = (cx > position.x - halfW && cx < position.x + halfW);
    bool insideZ = (cz > position.z - 50 && cz < position.z + 50);
    bool insideY = (fabs(cy - currentY) < 30.0f);

    return insideX && insideZ && insideY;
}

void Elevator::draw() {
    glPushMatrix();
    glTranslatef(position.x, currentY, position.z);

    // جسم المصعد
    glColor3f(0.2f, 0.2f, 0.2f);
    Cuboid(Point(0, 50, 0), 100, 100, 120).draw();

    // الباب الأيسر
    glColor3f(0.1f, 0.1f, 0.1f);
    Cuboid(
        Point(-doorWidth / 2, 50, 60),
        100,
        5,
        maxDoorWidth / 2 - doorWidth / 2
    ).draw();

    // الباب الأيمن
    Cuboid(
        Point(doorWidth / 2, 50, 60),
        100,
        5,
        maxDoorWidth / 2 - doorWidth / 2
    ).draw();

    glPopMatrix();
}

