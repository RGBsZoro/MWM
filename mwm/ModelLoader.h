#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <vector>
#include <string>
#include <map>
#include <GL/freeglut.h>
#include "Point.h"

struct Material {
    float r, g, b;
};

struct Face {
    std::vector<int> vertexIndices;
};

struct Group {
    std::string materialName;
    std::vector<Face> faces;
};

class ModelLoader {
public:
    std::vector<Point> vertices;
    std::vector<Group> groups; // تم التغيير لاستخدام المجموعات بدلاً من الأوجه المنفردة
    std::map<std::string, Material> materials;

    // دالة التحميل المعدلة لتقبل ملفين
    bool load(const std::string& objFile, const std::string& mtlFile);

    // دالة الرسم النهائية
    void draw();
};

#endif