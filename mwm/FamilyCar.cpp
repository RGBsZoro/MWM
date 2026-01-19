#include "FamilyCar.h"
#include "Sofa.h"
#include "Cylinder.h"
void FamilyCar::draw() {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(carRotation, 0, 1, 0);

    // ==========================================
// 1. الأساسيات (عجلات وقاعدة) - مع تصغير الدواليب
// ==========================================
    float scale = 0.75f; // معامل التصغير (غيره كما تحب، مثلاً 0.8)
    float smallRadius = wheelRadius * scale;
    float wheelWidth = width * 0.15f;
    float xDist = length * 0.35f;
    float zDist = width / 2.0f;

    // رسم العجلات
    float wPos[4][3] = {
        // استخدمنا smallRadius هنا في المحور Y لتظل العجلة ملامسة للأرض
        { xDist, smallRadius,  zDist},
        { xDist, smallRadius, -zDist - wheelWidth},
        {-xDist, smallRadius,  zDist},
        {-xDist, smallRadius, -zDist - wheelWidth}
    };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(wPos[i][0], wPos[i][1], wPos[i][2]);

        // نرسل نصف القطر الصغير للكلاس ليرسم جنط وإطار أصغر
        wheel.draw(smallRadius, wheelWidth);

        glPopMatrix();
    }

    // القاعدة (مرفوعة قليلاً لتكون فوق العجلات)
    glColor3ub(30, 30, 30);
    // جعلنا القاعدة تمتد لتغطي كامل الطول وتخفي الفراغات السفلية
    float chassisLen = length * 0.89f;
    Cuboid base(Point(0, wheelRadius + 2.0f, 0), 4.0f, width - 2.0f, chassisLen);
    base.draw();

    // رسم الكراسي والتابلوه (موجود في كودك الأصلي)
    drawInterior();

    // ==========================================
    // 2. الهيكل الثابت (SKELETON) - هذا هو السر
    // هذا الجزء لا يتحرك ويحمل السقف ويسد الفراغات
    // ==========================================

    // قياسات الارتفاع (عدنا للقياسات العالية عشان الكراسي)
    float doorY = wheelRadius - 2.0f; // رفعنا الباب قليلاً
    float doorH = 16.0f;              // ارتفاع الباب المعدني
    float winH = 22.0f;               // ارتفاع الزجاج (عالي عشان الراس)
    float roofH = doorY + doorH + winH; // سقف عالي

    float sideDist = width / 2.0f;
    float roofInward = 2.5f;          // ميلان خفيف للسقف

    // نقاط الحدود (مهمة جداً لضبط الحركة)
    float windshieldStart = length * 0.28f; // بداية الزجاج الأمامي (خلف الدولاب)
    float bPillarX = -length * 0.05f;       // مكان العمود الوسطي
    float rearWindowStart = -length * 0.45f;// بداية الزجاج الخلفي
    float backDoorEnd = -xDist + 2.0f;

    // الصندوق الخلفي
    float trunkEnd = -length * 0.55f;
    float trunkH = doorY + doorH + 7.0f; // تم رفعه قليلاً ليتناسب مع الزجاج
    glPushMatrix();
    // أ. العمود الوسطي (B-Pillar) - عمود أسود يفصل بين البابين
    glColor3ub(40, 40, 40);
    float pillarW = 2.0f; // عرض العمود

    // عمود يمين
    glBegin(GL_QUADS);
    glVertex3f(bPillarX + pillarW, doorY + 4, sideDist - 0.5f);
    glVertex3f(bPillarX - pillarW, doorY + 4, sideDist - 0.5f);
    glVertex3f(bPillarX - pillarW, roofH, sideDist - roofInward);
    glVertex3f(bPillarX + pillarW, roofH, sideDist - roofInward);
    glEnd();

    // عمود يسار
    glBegin(GL_QUADS);
    glVertex3f(bPillarX + pillarW, doorY + 4, -sideDist + 0.5f);
    glVertex3f(bPillarX - pillarW, doorY + 4, -sideDist + 0.5f);
    glVertex3f(bPillarX - pillarW, roofH, -sideDist + roofInward);
    glVertex3f(bPillarX + pillarW, roofH, -sideDist + roofInward);
    glEnd();

    // ب. السقف (Roof) - يمتد فوق الجميع
    glColor3ub(50, 50, 60);
    glBegin(GL_QUADS);
    // السقف يربط بين أعلى الزجاج الأمامي وأعلى الزجاج الخلفي
    glVertex3f(windshieldStart, roofH, sideDist - roofInward);
    glVertex3f(windshieldStart, roofH, -sideDist + roofInward);
    glVertex3f(rearWindowStart, roofH, -sideDist + roofInward);
    glVertex3f(rearWindowStart, roofH, sideDist - roofInward);
    glEnd();
    glPopMatrix();

    // ==========================================
    // 3. الأبواب (المتحركة) - تم ضبط مكانها لمنع التداخل
    // ==========================================

    float doorThick = 1.5f;
    // طول الباب الأمامي: من الزجاج الأمامي إلى العمود الوسط
    float frontDoorLen = (windshieldStart - (bPillarX + pillarW));
    // --- باب السائق ---
    glPushMatrix();
    // نقطة التمفصل (Hinge) عند بداية الزجاج الأمامي وليس عند العجل
    glTranslatef(windshieldStart, doorY, sideDist);
    glRotatef(doorAngle, 0, 1, 0);

    // جسم الباب (يرسم بالسالب أي للخلف بالنسبة لنقطة التمفصل)
    glColor3ub(70, 70, 80);
    Cuboid dDoor(Point(-frontDoorLen / 2.0f, doorH / 2.0f, 0), doorH, doorThick, frontDoorLen);
    dDoor.draw();

    // زجاج السائق (شفاف)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.8f, 0.9f, 1.0f, 0.4f);
    glBegin(GL_QUADS);
    glVertex3f(0, doorH, 0);                 // أمامي أسفل
    glVertex3f(-frontDoorLen, doorH, 0);     // خلفي أسفل
    glVertex3f(-frontDoorLen, doorH + winH, -roofInward); // خلفي أعلى
    glVertex3f(0, doorH + winH, -roofInward);  // أمامي أعلى
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();

    // --- باب الراكب ---
    glPushMatrix();
    glTranslatef(windshieldStart, doorY, -sideDist);
    // (يمكنك إضافة glRotatef هنا إذا أردت فتح باب الراكب أيضاً)
    glColor3ub(70, 70, 80);
    Cuboid pDoor(Point(-frontDoorLen / 2.0f, doorH / 2.0f, 0), doorH, doorThick, frontDoorLen);
    pDoor.draw();

    // زجاج الراكب
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

    // --- الأبواب الخلفية (ثابتة أو تفتح حسب رغبتك) ---
    // تبدأ بعد العمود الوسطي
    float backDoorStart = bPillarX - pillarW;
    float backDoorLen = (backDoorStart - rearWindowStart);

    // يسار خلفي
    glPushMatrix();
    glTranslatef(backDoorStart - backDoorLen / 2.0f, doorY + doorH / 2.0f, sideDist);
    glColor3ub(65, 65, 75);
    Cuboid rlDoor(Point(0, 0, 0), doorH, doorThick, backDoorLen);
    rlDoor.draw();
    glPopMatrix();

    // زجاج يسار خلفي (يدوي لضبط الميلان)
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

    // يمين خلفي (نفس المنطق)
    glPushMatrix();
    glTranslatef(backDoorStart - backDoorLen / 2.0f, doorY + doorH / 2.0f, -sideDist);
    glColor3ub(65, 65, 75);
    Cuboid rrDoor(Point(0, 0, 0), doorH, doorThick, backDoorLen);
    rrDoor.draw();
    glPopMatrix();

    // زجاج يمين خلفي
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

    // ==========================================
    // 4. إغلاق الفجوات (الزجاج الأمامي، الخلفي، الغطاء)
    // ==========================================

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // الزجاج الأمامي (Windshield)
    glColor4f(0.7f, 0.85f, 1.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(windshieldStart + 15.0f, doorY + doorH + 9, sideDist); // قاعدة الزجاج (أمام الباب بقليل)
    glVertex3f(windshieldStart + 15.0f, doorY + doorH + 9, -sideDist);
    glVertex3f(windshieldStart, roofH, -sideDist + roofInward); // قمة الزجاج (عند السقف)
    glVertex3f(windshieldStart, roofH, sideDist - roofInward);
    glEnd();
    // الزجاج الخلفي
    // الزجاج الخلفي المسطح - ينزل عمودياً من السقف
    glBegin(GL_QUADS);
    glVertex3f(rearWindowStart, roofH, sideDist - roofInward);  // أعلى يمين
    glVertex3f(rearWindowStart, roofH, -sideDist + roofInward); // أعلى يسار
    glVertex3f(rearWindowStart, trunkH, -sideDist);             // أسفل يسار (عند الصندوق)
    glVertex3f(rearWindowStart, trunkH, sideDist);              // أسفل يمين (عند الصندوق)
    glEnd();
    glDisable(GL_BLEND);

    // ==========================================
    // 5. غطاء المحرك والمؤخرة (جسم السيارة)
    // ==========================================



// ==========================================
// --- غطاء المحرك المعدل (مطابق لإحداثيات الزجاج والتابلوه) ---
// ==========================================

// 1. مزامنة النقاط الطولية (X)
    float hoodBackX = windshieldStart + 15.0f; // نقطة التقاء الزجاج (ثابتة لضمان عدم وجود فجوات)
    float hoodFrontX = length * 0.58f;        // مقدمة السيارة

    // 2. ضبط الارتفاعات (Y) لضمان تغطية التابلوه ومنع الاختراق
    // قاعدة الزجاج والتابلوه عندك الآن تصل إلى doorY + doorH + 9
    float hoodHighY = doorY + doorH + 9.5f;   // أعلى من الزجاج بـ 0.5 لضمان التغطية التامة
    float hoodLowY = doorY + doorH + 2.0f;    // ارتفاع المقدمة عند الكسرة العلوي

    glPushMatrix();
    // لون الهيكل
    glColor3ub(60, 60, 70);

    // 1. السطح العلوي المائل للغطاء (The Main Hood Plane)
    glBegin(GL_POLYGON);
    glVertex3f(hoodFrontX, hoodLowY, sideDist);
    glVertex3f(hoodFrontX, hoodLowY, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, sideDist);
    glEnd();

    // 2. الحاجز العمودي الخلفي (عند الزجاج)
    // يسد الفراغ تماماً تحت الزجاج ويمنع رؤية التابلوه من الداخل
    glBegin(GL_QUADS);
    glVertex3f(hoodBackX, hoodHighY, sideDist);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(hoodBackX, doorY + doorH, -sideDist); // ينتهي عند خط الأبواب
    glVertex3f(hoodBackX, doorY + doorH, sideDist);
    glEnd();

    // 3. الواجهة الأمامية (الصدام/المستطيل الأمامي)
    // تنزل من hoodLowY إلى مستوى القاعدة لتغطية المقدمة
    glBegin(GL_QUADS);
    glVertex3f(hoodFrontX, hoodLowY, sideDist);
    glVertex3f(hoodFrontX, hoodLowY, -sideDist);
    glVertex3f(hoodFrontX, doorY + 1.0f, -sideDist); // فوق الأرضية بقليل
    glVertex3f(hoodFrontX, doorY + 1.0f, sideDist);
    glEnd();

    // 4. الجوانب (Fenders) - لسد الفجوات فوق العجلات
    // تربط بين ميلان الغطاء العلوي وخط الأبواب والأرضية
    glBegin(GL_QUADS);
    // --- الجانب الأيمن ---
    glVertex3f(hoodBackX, hoodHighY, sideDist);
    glVertex3f(hoodFrontX, hoodLowY, sideDist);
    glVertex3f(hoodFrontX, doorY + 1.0f, sideDist);
    glVertex3f(hoodBackX, doorY + 1.0f, sideDist);

    // --- الجانب الأيسر ---
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(hoodFrontX, hoodLowY, -sideDist);
    glVertex3f(hoodFrontX, doorY + 1.0f, -sideDist);
    glVertex3f(hoodBackX, doorY + 1.0f, -sideDist);
    glEnd();

    glPopMatrix();


    // ==========================================
    // 5. الصندوق الخلفي (Trunk) والزجاج الخلفي
    // ==========================================

    // تعريف نقطة نهاية الباب الخلفي (عند بداية الدولاب الخلفي)
    // xDist هو بعد الدواليب عن المركز، فالدولاب الخلفي عند -xDist


    // ==========================================
// 5. غطاء المحرك والمؤخرة (جسم السيارة)
// ==========================================

// --- أولاً: تأكد أن هذه الحسابات موجودة قبل الكود ---
    /*float hoodBackX = windshieldStart + 15.0f;
    float hoodHighY = doorY + doorH + 9.5f;
    float hoodLowY = doorY + doorH + 2.0f;
    float hoodFrontX = length * 0.58f;*/
    // ==========================================
// --- كود سد الفجوة الجانبية بالكامل (Side Fillers) ---
// ==========================================
    // ==========================================
// --- قطعة إغلاق الفجوة الجانبية بالكامل (Full Side Filler) ---
// ==========================================
    glPushMatrix();
    glColor3ub(60, 60, 70); // نفس لون الهيكل
    // الجانب الأيمن
    glBegin(GL_POLYGON);
    glVertex3f(windshieldStart, doorY + doorH - 8.0f, sideDist); // 1. نقطة منخفضة (تمديد لتحت)
    glVertex3f(hoodBackX, doorY + doorH - 8.0f, sideDist);      // 2. سحب المستطيل للأمام ولتحت
    glVertex3f(hoodBackX, hoodHighY, sideDist);                 // 3. الصعود لزاوية الغطاء
    glVertex3f(windshieldStart, roofH - winH, sideDist);        // 4. نقطة علوية لسد الفتحة فوق
    glVertex3f(windshieldStart - 2.0f, doorY + doorH, sideDist); // 5. العودة لزاوية الباب
    glEnd();

    // الجانب الأيسر
    glBegin(GL_POLYGON);
    glVertex3f(windshieldStart, doorY + doorH - 8.0f, -sideDist);
    glVertex3f(hoodBackX, doorY + doorH - 8.0f, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(windshieldStart, roofH - winH, -sideDist);
    glVertex3f(windshieldStart - 2.0f, doorY + doorH, -sideDist);
    glEnd();

    glPopMatrix();

    // ==========================================
 // --- قطعة الربط النهائية (The Final Connector Bridge) ---
 // ==========================================
    glPushMatrix();
    glColor3ub(60, 60, 70);

    glBegin(GL_QUADS);
    // --- الجانب الأيمن ---
    glVertex3f(windshieldStart, roofH, sideDist - roofInward);    // 1. زاوية السقف
    glVertex3f(hoodBackX, hoodHighY, sideDist);                  // 2. زاوية الغطاء
    glVertex3f(windshieldStart, hoodHighY, sideDist);            // 3. فوق زاوية الباب مباشرة
    glVertex3f(windshieldStart, roofH - winH, sideDist);         // 4. النقطة التي كانت تسبب الفجوة (نهاية القطعة السفلية)

    // --- الجانب الأيسر ---
    glVertex3f(windshieldStart, roofH, -sideDist + roofInward);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(windshieldStart, hoodHighY, -sideDist);
    glVertex3f(windshieldStart, roofH - winH, -sideDist);
    glEnd();

    glPopMatrix();

    // ------------------------------------------------
    // --- ثالثاً: يليه فوراً كود غطاء المحرك (The Main Hood) ---
    // ------------------------------------------------
    glPushMatrix();
    glColor3ub(60, 60, 70);
    glBegin(GL_POLYGON); // السطح العلوي المائل
    glVertex3f(hoodFrontX, hoodLowY, sideDist);
    glVertex3f(hoodFrontX, hoodLowY, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, -sideDist);
    glVertex3f(hoodBackX, hoodHighY, sideDist);
    glEnd();
    // ... باقي كود الغطاء (الحاجز العمودي، الواجهة، الجوانب) ...
    glPopMatrix();



    // ==========================================
// 5. الصندوق الخلفي (Trunk) - تصميم مائل قطعة واحدة
// ==========================================
// غطاء الظهر المعدني (الواجهة الخلفية العمودية)
    glBegin(GL_QUADS);
    glColor3ub(55, 55, 65); // نفس لون هيكل السيارة
    // تبدأ من مستوى الشنطة (تحت الزجاج) وتنتهي عند مستوى الباب/الأرضية
    glVertex3f(rearWindowStart, trunkH, sideDist);  // أعلى يمين
    glVertex3f(rearWindowStart, trunkH, -sideDist); // أعلى يسار
    glVertex3f(rearWindowStart, doorY + 3, -sideDist);  // أسفل يسار (عند الأرضية)
    glVertex3f(rearWindowStart, doorY + 3, sideDist);   // أسفل يمين (عند الأرضية)
    glEnd();




    // --- الأضواء الأمامية ---
    glPushMatrix();
    glColor3ub(255, 255, 220); // لون أبيض مائل للأصفر (كشافات)
    float lightSize = 3.0f;
    float lightY = doorY + 6.0f; // ارتفاع الضوء عن الأرض

    glBegin(GL_QUADS);
    // الضوء الأيمن
    glVertex3f(hoodFrontX + 0.1f, lightY + lightSize, sideDist - 2.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY - lightSize, sideDist - 2.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY - lightSize, sideDist - 7.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY + lightSize, sideDist - 7.0f);

    // الضوء الأيسر
    glVertex3f(hoodFrontX + 0.1f, lightY + lightSize, -sideDist + 2.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY - lightSize, -sideDist + 2.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY - lightSize, -sideDist + 7.0f);
    glVertex3f(hoodFrontX + 0.1f, lightY + lightSize, -sideDist + 7.0f);
    glEnd();
    glPopMatrix();






    // --- الأضواء الخلفية ---
    glPushMatrix();
    glColor3ub(200, 0, 0); // لون أحمر غامق
    float tailLightY = doorY + 8.0f;
    glBegin(GL_QUADS);
    // الضوء الخلفي الأيمن
    glVertex3f(rearWindowStart - 0.1f, tailLightY + 2.5f, sideDist - 1.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY - 2.5f, sideDist - 1.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY - 2.5f, sideDist - 6.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY + 2.5f, sideDist - 6.0f);

    // الضوء الخلفي الأيسر
    glVertex3f(rearWindowStart - 0.1f, tailLightY + 2.5f, -sideDist + 1.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY - 2.5f, -sideDist + 1.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY - 2.5f, -sideDist + 6.0f);
    glVertex3f(rearWindowStart - 0.1f, tailLightY + 2.5f, -sideDist + 6.0f);
    glEnd();
    glPopMatrix();



    // --- المرايا الجانبية (ثابتة على الهيكل) ---
    glPushMatrix();
    glColor3ub(40, 40, 40); // لون المرايا (أسود أو رمادي غامق)

    // المرآة اليمنى (جهة السائق)
    glPushMatrix();
    // نضعها عند بداية الزجاج وعلى ارتفاع مناسب فوق خط الباب
    glTranslatef(windshieldStart + 2.0f, doorY + doorH + 1.0f, sideDist + 0.5f);
    Cuboid mirrorR(Point(0, 8.0f, 1.0f), 2.0f, 3.0f, 1.2f); // جسم المرآة
    mirrorR.draw();
    glPopMatrix();

    // المرآة اليسرى (جهة الراكب)
    glPushMatrix();
    glTranslatef(windshieldStart + 2.0f, doorY + doorH + 1.0f, -sideDist - 0.5f);
    Cuboid mirrorL(Point(0, 8.0f, -1.0f), 2.0f, 3.0f, 1.2f);
    mirrorL.draw();
    glPopMatrix();

    glPopMatrix();


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

    //  ضبط ذراع المقود 
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

    //glPopMatrix();

    glPopMatrix();
}
