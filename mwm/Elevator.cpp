#include <windows.h>   // يجب أن يكون في الأعلى
#include "Elevator.h"
#include <cmath>
#include "FamilyCar.h"

// ===============================================================
// الـ Constructor: تهيئة المصعد في وضع السكون عند الطابق الأرضي
// ===============================================================
Elevator::Elevator(Point startPos, float targetH)
    : position(startPos),
      bottomY(startPos.y),                // مستوى الأرضية الأرضي (مثلاً 0)
      topY(startPos.y + targetH),         // مستوى الأرضية العلوي
      currentY(startPos.y),               // البداية من الأرض تماماً
      speed(1.5f),                        // سرعة الحركة
      cabinW(380.0f),                     // عرض الكبينة (مطابق للفتحة التي صنعناها)
      cabinD(400.0f),                     // عمق الكبينة
      cabinH(260.0f),                     // ارتفاع الكبينة
      doorState(DoorState::CLOSED),
      doorWidth(0.0f),                    // مقدار الانفتاح (0 = مغلق)
      maxDoorWidth(170.0f),               // المسافة التي يتحركها كل باب (نصف العرض تقريباً)
      state(ElevatorState::IDLE) 
{}

// ===============================================================
// تحديث منطق الحركة والأبواب
// ===============================================================
void Elevator::update(Camera& cam, FamilyCar& car) {
    // 1. منطق الأبواب
    if (doorState == DoorState::OPENING) {
        doorWidth += 2.5f;
        if (doorWidth >= maxDoorWidth) { doorWidth = maxDoorWidth; doorState = DoorState::OPEN; }
    }
    else if (doorState == DoorState::CLOSING) {
        doorWidth -= 2.5f;
        if (doorWidth <= 0.0f) {
            doorWidth = 0.0f; doorState = DoorState::CLOSED;
            // يبدأ المصعد بالحركة إذا كان اللاعب أو السيارة بالداخل
            if (isPlayerInside(cam) || isCarInside(car)) {
                if (fabs(currentY - bottomY) < 5.0f) state = ElevatorState::MOVING_UP;
                else state = ElevatorState::MOVING_DOWN;
            }
        }
    }

    // 2. حركة المصعد وتحديث الأجسام
    if (state != ElevatorState::IDLE && doorState == DoorState::CLOSED) {
        float oldY = currentY;

        if (state == ElevatorState::MOVING_UP) {
            currentY += speed;
            if (currentY >= topY) {
                currentY = topY;
                state = ElevatorState::IDLE;
                doorState = DoorState::OPENING;
                PlaySound(TEXT("Sounds/elevator_ding.wav"), NULL, SND_FILENAME | SND_ASYNC);
            }
        }
        else if (state == ElevatorState::MOVING_DOWN) {
            currentY -= speed;
            if (currentY <= bottomY) {
                currentY = bottomY;
                state = ElevatorState::IDLE;
                doorState = DoorState::OPENING;
                PlaySound(TEXT("Sounds/elevator_ding.wav"), NULL, SND_FILENAME | SND_ASYNC);
            }
        }

        // حساب الإزاحة (deltaY) بعد تحديث currentY
        float deltaY = currentY - oldY;

        // تحديث اللاعب
        if (isPlayerInside(cam)) {
            cam.MoveWithPlatform(deltaY, currentY);
        }

        // تحديث السيارة (لاحظ استخدام car.pos.y بناءً على كود FamilyCar)
        if (isCarInside(car)) {
            car.pos.y += deltaY;

            // منع السيارة من الغوص داخل أرضية المصعد
            if (car.pos.y < currentY) {
                car.pos.y = currentY;
            }
        }
    }
}// ===============================================================
// استدعاء المصعد (عند الضغط على مفتاح L)
// ===============================================================
void Elevator::callElevator(Camera& cam, float targetZ) {
    float cx, cy, cz;
    cam.GetPos(cx, cy, cz);

    // التحقق من قرب اللاعب من المصعد (أمام الفتحة)
    bool nearShaft = fabs(cx - position.x) < 200.0f && fabs(cz - targetZ) < 150.0f;
    if (!nearShaft) return;

    if (doorState == DoorState::OPEN) {
        doorState = DoorState::CLOSING;
    } 
    else if (doorState == DoorState::CLOSED && state == ElevatorState::IDLE) {
        // تحديد مكان اللاعب بالنسبة للمصعد
        float playerY = cy;
        float midPoint = (bottomY + topY) / 2.0f;

        if (playerY < midPoint) { // اللاعب في الطابق الأرضي
            if (fabs(currentY - bottomY) < 5.0f) doorState = DoorState::OPENING;
            else state = ElevatorState::MOVING_DOWN;
        } else { // اللاعب في الطابق العلوي
            if (fabs(currentY - topY) < 5.0f) doorState = DoorState::OPENING;
            else state = ElevatorState::MOVING_UP;
        }
    }
}

// ===============================================================
// التحقق من وجود اللاعب داخل الكبينة (للتصادم والحركة)
// ===============================================================
bool Elevator::isCarInside(FamilyCar& car) {
    // التحقق من X و Z
    bool insideX = (car.pos.x >= position.x - cabinW / 2.0f) &&
        (car.pos.x <= position.x + cabinW / 2.0f);

    bool insideZ = (car.pos.z <= position.z) &&
        (car.pos.z >= position.z - cabinD);

    // التحقق من الارتفاع Y (مع هامش بسيط)
    bool insideY = (car.pos.y >= currentY - 10.0f) &&
        (car.pos.y <= currentY + cabinH);

    return insideX && insideZ && insideY;
}
bool Elevator::isPlayerInside(Camera& cam) {
    float cx, cy, cz;
    cam.GetPos(cx, cy, cz);

    // التحقق من الحدود الأفقية (X, Z)
    bool insideX = (cx >= position.x - cabinW / 2.0f) && (cx <= position.x + cabinW / 2.0f);
    bool insideZ = (cz <= position.z) && (cz >= position.z - cabinD);

    // التحقق من الارتفاع (Y): يجب أن يكون اللاعب فوق الأرضية وتحت السقف
    // السماح بهامش بسيط (20 وحدة) للجاذبية
    bool insideY = (cy >= currentY + 10.0f) && (cy <= currentY + cabinH + 50.0f);

    return insideX && insideZ && insideY;
}

// ===============================================================
// الرسم (الكبينة، الأبواب، الإضاءة، لوحة التحكم)
// ===============================================================
void Elevator::draw() {
    glPushMatrix();
    glTranslatef(position.x, currentY, position.z);

    float floorThickness = 2.0f;

    // 1. الأرضية
    glColor3f(0.15f, 0.15f, 0.15f); // رمادي غامق جداً
    Cuboid(Point(0, 0, -cabinD / 2.0f), floorThickness, cabinD, cabinW).draw();

    // 2. السقف (الجديد)
    glColor3f(0.3f, 0.3f, 0.3f);
    Cuboid(Point(0, cabinH, -cabinD / 2.0f), 2.0f, cabinD, cabinW).draw();

    // 3. الأبواب
    float doorH = 225.0f;
    float doorThickness = 4.0f;
    glColor3f(0.5f, 0.5f, 0.55f); // لون معدني فاتح

    glPushMatrix();
    glTranslatef(-(doorWidth), floorThickness, 0);
    Cuboid(Point(-cabinW / 4, 0, 0), doorH, doorThickness, cabinW / 2).draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(doorWidth, floorThickness, 0);
    Cuboid(Point(cabinW / 4, 0, 0), doorH, doorThickness, cabinW / 2).draw();
    glPopMatrix();

    // 4. الجدران
    glColor3f(0.25f, 0.25f, 0.28f);
    Cuboid(Point(0, 0, -cabinD), cabinH, 5, cabinW).draw(); // خلفي
    Cuboid(Point(-cabinW / 2, 0, -cabinD / 2), cabinH, cabinD, 5).draw(); // يسار
    Cuboid(Point(cabinW / 2, 0, -cabinD / 2), cabinH, cabinD, 5).draw(); // يمين

    // 5. الإضافات (الأزرار والأنوار)
    drawCeilingLight();
    drawControlPanel();
    drawFloorIndicator();

    glPopMatrix();
}

void Elevator::drawControlPanel() {
    // قاعدة اللوحة (معدن مصقول)
    glColor3f(0.6f, 0.6f, 0.65f);
    float panelX = cabinW / 2 - 6; // ملتصقة بالجدار الأيمن
    Cuboid(Point(panelX, 100, -cabinD / 4), 80, 40, 2).draw();

    // زر الطابق الأرضي (G) - لونه أحمر إذا كان المصعد هناك
    if (fabs(currentY - bottomY) < 5.0f) glColor3f(1.0f, 0.0f, 0.0f);
    else glColor3f(0.2f, 0.2f, 0.2f);
    Cuboid(Point(panelX - 1, 110, -cabinD / 4), 15, 15, 3).draw();

    // زر الطابق الأول (1) - لونه أخضر إذا كان المصعد هناك
    if (fabs(currentY - topY) < 5.0f) glColor3f(0.0f, 1.0f, 0.0f);
    else glColor3f(0.2f, 0.2f, 0.2f);
    Cuboid(Point(panelX - 1, 140, -cabinD / 4), 15, 15, 3).draw();
}
void Elevator::drawCeilingLight() {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.8f);
    Cuboid(Point(0, cabinH - 2, -cabinD / 2), 1, cabinD / 3, cabinW / 3).draw();
    glEnable(GL_LIGHTING);
}

void Elevator::drawFloorIndicator() {
    // مؤشر رقم الطابق فوق الباب
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.0f, 0.0f);
    // هنا يمكن إضافة نص "G" أو "1" باستخدام glutStrokeCharacter
    glEnable(GL_LIGHTING);
}