#pragma once
#include "Point.h"
#include "Cuboid.h"
#include <GL/freeglut.h>
#include <cmath>

struct Door {
    // يجب أن تكون هذه القيم مطابقة تماماً لما في drawDetailedBuilding
    Point center = Point(0, 0.0f, 750);
    float totalWidth = 500.0f;
    float height = 150.0f;            // نفس ارتفاع فتحة الباب doorH
    float thickness = 2.0f;

    float slideOffset = 0.0f;
    bool isOpen = false;

    void draw() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float panelWidth = totalWidth / 2.0f;

        // رسم الضلفة اليمنى
        drawPanel(panelWidth, 1);
        // رسم الضلفة اليسرى
        drawPanel(panelWidth, -1);

        glDisable(GL_BLEND);
    }

    void drawPanel(float w, int direction) {
        glPushMatrix();

        //posX: يبقى كما هو (إزاحة جانبية)
        //posY: بما أن الباب يرتفع من المنتصف، سنضعه عند الأرض مباشرة (-3) 
        // وبما أنك قلت أنه يرتفع لنصف المبنى، فسنلغي الـ (height/2)
        float posX = center.x + (direction * (w / 2 + slideOffset));
        float posY = center.y; // اجعلها تبدأ من الأرض مباشرة (-3)
        float posZ = center.z;

        glTranslatef(posX, posY, posZ);

        // 1. الزجاج الشفاف
        glColor4f(0.4f, 0.7f, 1.0f, 0.4f);
        // تأكد أن أبعاد الـ Cuboid هنا هي (height, thickness, w)
        Cuboid glass(Point(0, 0, 0), height, thickness, w);
        glass.draw();

        // 2. إطار معدني جانبي (اختياري)
        glDisable(GL_BLEND);
        glColor3f(0.8f, 0.8f, 0.8f);
        // رسم خط نحيف يمثل إطار الباب
        Cuboid frame(Point(0, 0, 0), height, thickness + 0.5f, 2.0f);
        frame.draw();
        glEnable(GL_BLEND);

        glPopMatrix();
    }

    void update(float camX, float camZ) {
        float dist = sqrt(pow(camX - center.x, 2) + pow(camZ - center.z, 2));
        isOpen = (dist < 350.0f);

        // maxSlide يجب أن يكون عرض الضلفة الواحدة لكي يختفي الزجاج خلف الجدران تماماً
        float maxSlide = (totalWidth / 2.0f) - 5.0f;
        if (isOpen && slideOffset < maxSlide) slideOffset += 10.0f;
        if (!isOpen && slideOffset > 0.0f) slideOffset -= 10.0f;
    }
};
