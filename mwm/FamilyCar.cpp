#include "FamilyCar.h"
#include "Sofa.h"
#include "Cylinder.h"
void FamilyCar::draw() {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(carRotation, 0, 1, 0);

    // 1. حسابات الدواليب بناءً على الأبعاد الممررة
    float wheelWidth = width * 0.15f; // عرض الدولاب يتناسب مع عرض السيارة
    float xDist = length * 0.35f;     // المسافة بين المحاور
    float zDist = width / 2.0f;       // المسافة بين الدواليب

    float wPos[4][3] = {
        { xDist, wheelRadius,  zDist},
        { xDist, wheelRadius, -zDist - wheelWidth},
        {-xDist, wheelRadius,  zDist},
        {-xDist, wheelRadius, -zDist - wheelWidth}
    };

    for (int i = 0; i < 4; i++){
        glPushMatrix();
        glTranslatef(wPos[i][0], wPos[i][1], wPos[i][2]);
        wheel.draw(wheelRadius, wheelWidth);
        glPopMatrix();
    }

    // 2. رسم الشاسيه (القاعدة)
    // القاعدة ترتفع دائماً بمقدار نصف قطر الدولاب لضمان عدم التداخل
    glColor3ub(30, 30, 30);
    float baseThickness = 4.0f;
    // رسم القاعدة فوق مستوى المحاور بقليل
    Cuboid base(Point(0, wheelRadius + 2.0f, 0), baseThickness, width - 4.0f, length - 10.0f);
    base.draw();

    drawInterior();

    glPopMatrix();
}

void FamilyCar::drawInterior() {
    // تعريف أداة رسم الأسطوانة
    Cylinder cylinderTool;

    // إحداثيات عامة
    float seatY = wheelRadius + 5.0f;
    float fSeatX = length * 0.1f;
    float fSeatW = 18.0f;
    float fBaseL = 18.0f;
    float commonBackH = 22.0f;

    // --- 1. رسم المقاعد الأمامية ---
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

    // --- 2. الكنبة الخلفية ---
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

    // ==========================================
    // --- 3. مقصورة القيادة (باستخدام كلاس Cylinder) ---
    // ==========================================

    float dashX = length * 0.35f;
    float dashH = 14.0f;
    float dashW = width - 4.0f;

    // أ. التابلوه الرئيسي
    glPushMatrix();
    glTranslatef(dashX, seatY + 4.0f, 0);
    glRotatef(-15, 0, 0, 1);
    glColor3ub(20, 20, 20);
    Cuboid dashboard(Point(0, 0, 0), dashH, dashW, 12.0f);
    dashboard.draw();
    glPopMatrix();

    // ب. المقود (Steering Wheel)
    glPushMatrix();
    // تقريب المقود للخلف باتجاه السائق: غيرنا -8.0f إلى -15.0f (أو أكثر حسب الرغبة)
    glTranslatef(dashX - 15.0f, seatY + 12.0f, width * 0.25f);

    glRotatef(90, 0, 1, 0);
    glRotatef(20, 1, 0, 0);

    glColor3ub(10, 10, 10);
    glutSolidTorus(1.0, 4.5, 15, 30);

    // ** ضبط ذراع المقود **
    glColor3ub(30, 30, 30);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    // زدنا الارتفاع (height) لـ 22.0f ليعوض المسافة الجديدة ويصل للتابلوه
    cylinderTool.draw(Point(0, 0.6, 0), 1.0f, 2.5f, 15.0f, 15, 10);
    glPopMatrix();

    glPopMatrix();

    // ج. لوحة العدادات
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

    // د. الكونسول والجير (باستخدام كلاس Cylinder)
    glPushMatrix();
    glTranslatef(fSeatX + 8.0f, seatY, 0);
    glColor3ub(25, 25, 25);
    Cuboid console(Point(0, 1.0f, 0), 4.0f, 8.0f, 15.0f);
    console.draw();

    // ننتقل لمكان خروج العصا من الكونسول
    glTranslatef(4.0f, 3.5f, 0);

    // 1. رسم العصا (تأكدنا أنها عمودية)
    glPushMatrix();
    glColor3ub(180, 180, 180);
    // التدوير -90 على X ليجعل أسطوانة كلاسك تقف للأعلى بدلاً من التمدد بالعرض
    glRotatef(0, 1, 0, 0);
    // رسم الأسطوانة عند نقطة الأصل المحلية (0,0,0)
    cylinderTool.draw(Point(0, 5, 0), 0.5f, 0.5f, 6.0f, 10, 5);
    glPopMatrix();

    // 2. رسم الكرة (الآن سنرفعها عمودياً على محور Y)
    glPushMatrix();
    // بما أن العصا طولها 6، نرفع الكرة 6 وحدات بالضبط على Y
    glTranslatef(0, 6.0f, 0);
    glColor3ub(10, 10, 10);
    glutSolidSphere(1.8, 15, 15);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}

