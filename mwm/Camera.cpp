#include "Camera.h"
#include <GL/glut.h>
#include <cmath>

void Camera::Init() {
    m_yaw = 4.44f;
    m_pitch = 0.0f;
    m_mode = MovementMode::WALK;
    m_lastSpaceTapTime = 0;
    m_verticalVelocity = 0.0f;
    m_isGrounded = true;

    // موقع البداية الافتراضي
    SetPos(0.0f, GROUND_Y + PLAYER_HEIGHT, 500.0f);
}

void Camera::Refresh() {
    m_lx = cos(m_yaw) * cos(m_pitch);
    m_ly = sin(m_pitch);
    m_lz = sin(m_yaw) * cos(m_pitch);

    m_strafe_lx = cos(m_yaw - M_PI_2);
    m_strafe_lz = sin(m_yaw - M_PI_2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_x, m_y, m_z, m_x + m_lx, m_y + m_ly, m_z + m_lz, 0.0, 1.0, 0.0);
}

bool Intersect(const AABB& a, const AABB& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x &&
        a.min.y <= b.max.y && a.max.y >= b.min.y &&
        a.min.z <= b.max.z && a.max.z >= b.min.z);
}

bool Camera::CheckCollision(const Point& newPos) {
    AABB playerBox = GetPlayerAABB(newPos);
    for (const auto& wall : walls) {
        if (Intersect(playerBox, { wall.min, wall.max })) return true;
    }
    return false;
}

bool Camera::CheckDoorCollision(const Point& newPos) {
    AABB playerBox = GetPlayerAABB(newPos);
    for (size_t i = 0; i < doorWalls.size(); i++) {
        if (i < Doors.size() && Doors[i] != nullptr && !Doors[i]->isOpen) {
            if (Intersect(playerBox, { doorWalls[i].min, doorWalls[i].max })) return true;
        }
    }
    return false;
}

bool Camera::CheckGroundCollision(const Point& newPos) {
    return newPos.y < GROUND_Y + PLAYER_HEIGHT;
}

// دالة التسلق: تحاول رفع اللاعب تدريجياً عند الاصطدام بحافة
bool Camera::TryStepUp(float nextX, float nextZ) {
    for (float h = 1.0f; h <= STEP_HEIGHT; h += 1.0f) {
        Point testPos(nextX, m_y + h, nextZ);
        if (!CheckCollision(testPos) && !CheckDoorCollision(testPos)) {
            m_x = nextX;
            m_z = nextZ;
            m_y += h; // الصعود للدرجة
            return true;
        }
    }
    return false;
}

void Camera::Move(float incr) {
    float nextX = m_x + incr * m_lx;
    float nextZ = m_z + incr * m_lz;
    Point nextPos(nextX, m_y, nextZ);

    if (!CheckCollision(nextPos) && !CheckDoorCollision(nextPos)) {
        m_x = nextX;
        m_z = nextZ;
        if (m_mode == MovementMode::FLY) m_y += incr * m_ly;
    }
    else if (m_mode == MovementMode::WALK) {
        TryStepUp(nextX, nextZ);
    }
    Refresh();
}

void Camera::Strafe(float incr) {
    float nextX = m_x + incr * m_strafe_lx;
    float nextZ = m_z + incr * m_strafe_lz;
    Point nextPos(nextX, m_y, nextZ);

    if (!CheckCollision(nextPos) && !CheckDoorCollision(nextPos)) {
        m_x = nextX;
        m_z = nextZ;
    }
    else if (m_mode == MovementMode::WALK) {
        TryStepUp(nextX, nextZ);
    }
    Refresh();
}

void Camera::ApplyGravity() {
    if (m_mode == MovementMode::FLY) return;

    m_verticalVelocity += GRAVITY;
    if (m_verticalVelocity < MAX_FALL_SPEED) m_verticalVelocity = MAX_FALL_SPEED;

    Point newPos(m_x, m_y + m_verticalVelocity, m_z);

    // فحص الأرضية الأساسية
    if (CheckGroundCollision(newPos)) {
        m_y = GROUND_Y + PLAYER_HEIGHT;
        m_verticalVelocity = 0.0f;
        m_isGrounded = true;
    }
    // فحص الوقوف فوق جدار أو منصة (Collision below feet)
    else if (CheckCollision(newPos) || CheckDoorCollision(newPos)) {
        m_verticalVelocity = 0.0f;
        m_isGrounded = true;
    }
    else {
        // حالة السقوط الحر
        m_y = newPos.y;
        m_isGrounded = false;
    }
    Refresh();
}

void Camera::Jump() {
    if (m_isGrounded && m_mode == MovementMode::WALK) {
        m_verticalVelocity = JUMP_FORCE;
        m_isGrounded = false;
    }
}

void Camera::HandleSpaceTap() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - m_lastSpaceTapTime < 300) {
        m_mode = (m_mode == MovementMode::WALK) ? MovementMode::FLY : MovementMode::WALK;
        if (m_mode == MovementMode::WALK) m_y = GROUND_Y + PLAYER_HEIGHT;
        m_verticalVelocity = 0;
    }
    else {
        Jump();
    }
    m_lastSpaceTapTime = currentTime;
}

AABB Camera::GetPlayerAABB(const Point& pos) {
    AABB box;
    box.min = Point(pos.x - PLAYER_RADIUS, pos.y - PLAYER_HEIGHT, pos.z - PLAYER_RADIUS);
    box.max = Point(pos.x + PLAYER_RADIUS, pos.y + 2.0f, pos.z + PLAYER_RADIUS);
    return box;
}

void Camera::Fly(float incr) {
    if (m_mode == MovementMode::FLY) {
        m_y += incr;
        Refresh();
    }
}

void Camera::RotateYaw(float angle) { m_yaw += angle; Refresh(); }
void Camera::RotatePitch(float angle) {
    m_pitch += angle;
    if (m_pitch < -1.5f) m_pitch = -1.5f;
    if (m_pitch > 1.5f) m_pitch = 1.5f;
    Refresh();
}

void Camera::SetPos(float x, float y, float z) { m_x = x; m_y = y; m_z = z; Refresh(); }
void Camera::GetPos(float& x, float& y, float& z) { x = m_x; y = m_y; z = m_z; }
void Camera::GetDirectionVector(float& x, float& y, float& z) { x = m_lx; y = m_ly; z = m_lz; }
void Camera::SetYaw(float angle) { m_yaw = angle; Refresh(); }
void Camera::SetPitch(float angle) { m_pitch = angle; Refresh(); }