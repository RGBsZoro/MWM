#include "ShowRoom.h"
#include "Cylinder.h"
#include "Elevator.h"

ShowRoom::ShowRoom() {}

// --- 1. إعداد الإضاءة الاحترافية (Spotlights) فوق كل منصة ---
void ShowRoom::setupPodiumLighting(int id, float x, float z) {
    GLfloat lightPos[] = { x, totalH - 20.0f, z, 1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f }; // ضوء دافئ قليلاً
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    int lightSource = GL_LIGHT1 + id;
    glEnable(lightSource);
    glLightfv(lightSource, GL_POSITION, lightPos);
    glLightfv(lightSource, GL_DIFFUSE, diffuse);
    glLightfv(lightSource, GL_SPECULAR, specular);

    GLfloat dir[] = { 0, -1, 0 }; // الاتجاه للأسفل نحو السيارة
    glLightfv(lightSource, GL_SPOT_DIRECTION, dir);
    glLightf(lightSource, GL_SPOT_CUTOFF, 45.0f);
    glLightf(lightSource, GL_SPOT_EXPONENT, 20.0f);
}

// --- 2. رسم المنصة الدائرية المتدرجة ---
void ShowRoom::drawModernPodium(float cx, float cz, int id) {
    float baseR = 180.0f;
    float tierH = 3.9f;
    Cylinder cycl;

    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 80.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    for (int i = 0; i < 3; i++) {
        float r = baseR - (i * 30.0f);
        float y = -3.0f + (i * tierH);

        // حلقة LED زرقاء تحت كل درجة
        glDisable(GL_LIGHTING);
        glColor3f(0.0f, 0.4f, 0.8f);
        cycl.draw(Point(cx, y + 0.2f, cz), r + 2, r + 2, 0.5f, 40, 5);
        glEnable(GL_LIGHTING);

        // جسم المنصة (رمادي داكن معدني)
        glColor3f(0.12f, 0.12f, 0.12f);
        cycl.draw(Point(cx, y + tierH, cz), r, r, tierH, 40, 5);
    }

    setupPodiumLighting(id, cx, cz);
}

// --- 3. توزيع المنصات (3 يمين و 3 يسار) مع حلقات السقف ---
void ShowRoom::drawPodiums() {
    float sideX = 400.0f;
    float spacingZ = 450.0f;
    int podiumID = 0;

    for (int row = 0; row < 3; row++) {
        float posZ = -450.0f + (row * spacingZ);

        // الجناح الأيمن
        drawModernPodium(sideX, posZ, podiumID++);
        drawCeilingLightRing(Point(sideX, totalH - 5, posZ));

        // الجناح الأيسر
        drawModernPodium(-sideX, posZ, podiumID++);
        drawCeilingLightRing(Point(-sideX, totalH - 5, posZ));
    }
}

// --- 4. حلقة ضوء السقف الدائرية ---
void ShowRoom::drawCeilingLightRing(Point p) {
    glPushMatrix();
    glTranslatef(p.x, p.y, p.z);
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 0.9f); // أبيض ساطع

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

// --- 5. مكتب الاستعلامات الخشبي (كاونتر وكرسي) ---
void ShowRoom::drawReceptionDesk() {
    glPushMatrix();
    // وضعه في المنتصف تماماً مقابل الباب (في جهة الجدار الخلفي)
    glTranslatef(0, -3, -totalL / 2 + 180);

    // الحاجز الخشبي الرئيسي
    glColor3ub(90, 50, 20); // خشب الجوز الداكن
    Cuboid body(Point(0, 30, 0), 60, 15, 200);
    body.draw();

    // سطح المكتب العريض
    glColor3ub(110, 70, 40);
    Cuboid top(Point(0, 60, 0), 5, 50, 220);
    top.draw();

    // الكرسي خلف المكتب
    glColor3ub(20, 20, 20);
    Cuboid seat(Point(0, 25, -60), 10, 40, 40);
    seat.draw();
    Cuboid backrest(Point(0, 65, -75), 50, 5, 40);
    backrest.draw();

    glPopMatrix();
}
void ShowRoom::drawFloorContent(float yOffset) {
    glPushMatrix();
    glTranslatef(0, yOffset, 0);

    // رسم المنصات الست (3 يمين و 3 يسار)
    drawPodiums();

    // رسم مكتب الاستعلامات (فقط في الطابق العلوي كما طلبت)
    if (yOffset > 0) {
        drawReceptionDesk();
    }

    glPopMatrix();
}

void ShowRoom::drawElevatorButton(float yOffset, bool isElevatorHere) {
    glPushMatrix();
    // نضع الزر على الجدار الخلفي بجانب فتحة المصعد تماماً
    glTranslatef(70, yOffset + 50, -totalL / 2 + 2);

    // إطار اللوحة
    glColor3f(0.1f, 0.1f, 0.1f);
    Cuboid(Point(0, 0, 0), 20, 2, 10).draw();

    // الزر المضيء
    glDisable(GL_LIGHTING);
    if (isElevatorHere)
        glColor3f(0.0f, 1.0f, 0.0f); // أخضر إذا كان المصعد واقفاً عندك
    else
        glColor3f(1.0f, 0.0f, 0.0f); // أحمر إذا كان في طابق آخر أو يتحرك

    Cuboid(Point(0, 0, 1), 5, 1, 5).draw();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// --- 6. الهيكل والاسم الذهبي والشارع ---
void ShowRoom::drawMWMName() {
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.84f, 0.0f); // ذهبي عيار 24
    glTranslatef(-85, 165, 755); // تم رفعه قليلاً ليتوسط المساحة فوق الباب
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
    float totalBuildingH = floorHeight * 2; // الطابقين معاً

    // الأرضية السفلى
    glColor3f(0.05f, 0.05f, 0.05f);
    Cuboid(Point(0, -3, 0), t, totalL, totalW).draw();

    // الأرضية الوسطى (سقف الأول وأرضية الثاني) مع فتحة للمصعد
    // سنرسمها كقطعتين لنترك مساحة للمصعد في الخلف
    Cuboid(Point(0, floorHeight - 3, 100), t, totalL - 200, totalW).draw();

    // السقف النهائي
    glColor3f(0.1f, 0.1f, 0.1f);
    Cuboid(Point(0, totalBuildingH - 3, 0), t, totalL, totalW).draw();

    // الجدران (يجب أن يصل ارتفاعها للضعف الآن)
    glColor3f(0.3f, 0.3f, 0.3f);
    // الجدار الخلفي الأيسر
    Cuboid(Point(-totalW / 2 + (totalW - 120) / 4, -3, -totalL / 2), totalBuildingH, 5, (totalW - 120) / 2).draw();
    // الجدار الخلفي الأيمن
    Cuboid(Point(totalW / 2 - (totalW - 120) / 4, -3, -totalL / 2), totalBuildingH, 5, (totalW - 120) / 2).draw();
    // قطعة فوق الباب في الطابق الأرضي
    Cuboid(Point(0, 100, -totalL / 2), floorHeight - 100, 5, 120).draw();    Cuboid(Point(totalW / 2, -3, 0), totalBuildingH, totalL, t).draw();  // Right
    // قطعة فوق الباب في الطابق العلوي (ليكتمل مظهر الفتحة)
    Cuboid(Point(0, floorHeight + 100, -totalL / 2), floorHeight - 100, 5, 120).draw();
    Cuboid(Point(-totalW / 2, -3, 0), totalBuildingH, totalL, t).draw(); // Left

    // الواجهة الأمامية مع الباب (الطابق الأرضي فقط)
    float sideWallW = (totalW - doorW) / 2.0f;
    float posX = (totalW / 2.0f) - (sideWallW / 2.0f);
    Cuboid(Point(-posX, -3, totalL / 2), floorHeight, t, sideWallW).draw();
    Cuboid(Point(posX, -3, totalL / 2), floorHeight, t, sideWallW).draw();
    Cuboid(Point(0, doorH - 3, totalL / 2), floorHeight - doorH, t, doorW).draw();
}
void ShowRoom::drawTree(float x, float z) {
    // 1. جذع الشجرة (بني)
    glColor3ub(101, 67, 33);
    Cuboid trunk(Point(x, 0.0f, z), 50.0f, 12.0f, 12.0f);
    trunk.draw();

    // 2. أوراق الشجرة (أخضر متدرج)
    // الطبقة السفلية الكبيرة
    glColor3ub(34, 139, 34);
    Cuboid leaves1(Point(x, 45.0f, z), 40.0f, 70.0f, 70.0f);
    leaves1.draw();

    // الطبقة الوسطى
    glColor3ub(46, 170, 46);
    Cuboid leaves2(Point(x, 75.0f, z), 35.0f, 55.0f, 55.0f);
    leaves2.draw();

    // الطبقة العلوية (القمة)
    glColor3ub(60, 200, 60);
    Cuboid leaves3(Point(x, 100.0f, z), 25.0f, 35.0f, 35.0f);
    leaves3.draw();
}

void ShowRoom::drawOppositeSideline() {
    float treeZ = 1450.0f;
    float totalLength = 3000.0f;

    // رسم رصيف بسيط تحت الشجر
    glColor3ub(50, 50, 50); // رمادي غامق
    Cuboid curb(Point(0, -1, treeZ), 2, 80, totalLength);
    curb.draw();

    // توزيع الأشجار على طول الرصيف
    float startX = -totalLength / 2;
    float step = 300.0f; // المسافة بين الشجر

    for (float x = startX + 100; x < totalLength / 2; x += step) {
        drawTree(x, treeZ);
    }
}
Point ShowRoom::GetElevatorShaftCenter() const {
    return Point(
        0.0f,
        -3.0f,
        -totalL / 2 + 2.5f   // داخل الجدار الخلفي تمامًا
    );
}

float ShowRoom::GetElevatorDoorWidth() const {
    return 120.0f;
}

float ShowRoom::GetFloorHeight() const {
    return floorHeight; // 300
}


void ShowRoom::drawStreet() {
    float startZ = 750.0f;
    float pieceLen = 50.0f;
    for (float x = -2000; x < 2000; x += pieceLen) {
        if (((int)(x / pieceLen)) % 2 == 0) glColor3ub(220, 220, 220);
        else glColor3ub(40, 40, 40);
        Cuboid(Point(x + pieceLen / 2, -1, startZ + 50), 2, 100, pieceLen).draw();
    }
    glColor3ub(15, 15, 15);
    Cuboid(Point(0, -2.5, startZ + 450), 1, 700, 4000).draw();
}

// --- 7. دالة الرسم الشاملة ---
void ShowRoom::draw() {
    drawStreet();

    // --- إضافة الأشجار على الطرف الآخر من الشارع ---
    drawOppositeSideline();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    drawBuildingBase();
    drawMWMName();

    // رسم الطابق الأرضي (yOffset = 0)
    drawFloorContent(0);

    // رسم الطابق العلوي (yOffset = floorHeight)
    // ملاحظة: تأكد أن قيمة floorHeight معرفة داخل الكلاس
    drawFloorContent(floorHeight);

    glDisable(GL_LIGHTING);
}
// --- 8. تحديث التصادمات ---
std::vector<Wall> ShowRoom::GetStaticWalls() {
    std::vector<Wall> w;
    float t = 5.0f;
    float totalBuildingH = 600.0f; // الارتفاع الكلي للطابقين

    // جدران المبنى (يجب أن تغطي الارتفاع الكلي)
    float shaftW = 120.0f;
    float sideW = (totalW - shaftW) / 2.0f;

    w.push_back(Cuboid(Point(-sideW / 2, -3, -totalL / 2),
        totalBuildingH, t, sideW).ToWall());

    w.push_back(Cuboid(Point(+sideW / 2, -3, -totalL / 2),
        totalBuildingH, t, sideW).ToWall());
    w.push_back(Cuboid(Point(totalW / 2, -3, 0), totalBuildingH, totalL, t).ToWall());
    w.push_back(Cuboid(Point(-totalW / 2, -3, 0), totalBuildingH, totalL, t).ToWall());

    // إضافة تصادم المنصات لكل طابق
    for (int floor = 0; floor < 2; floor++) {
        float yOff = floor * 300.0f;
        for (int row = 0; row < 3; row++) {
            float pz = -450.0f + (row * 450.0f);
            w.push_back(Cuboid(Point(400.0f, -3 + yOff, pz), 9, 300, 300).ToWall());
            w.push_back(Cuboid(Point(-400.0f, -3 + yOff, pz), 9, 300, 300).ToWall());
        }
    }

    // إضافة تصادم المكتب في الطابق العلوي فقط
    w.push_back(Cuboid(Point(0, 300 - 3, -totalL / 2 + 180), 60, 40, 220).ToWall());

    return w;
}
Wall ShowRoom::GetMainDoorWall() {
    return Cuboid(Point(0, -3, totalL / 2), doorH, 10.0f, doorW).ToWall();
}