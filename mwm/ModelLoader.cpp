#include "ModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>:"glass"

bool ModelLoader::load(const std::string& objFile, const std::string& mtlFile) {
    // 1. قراءة ملف الألوان MTL
    materials.clear();
    std::ifstream mFile(mtlFile);
    std::string line, currentMatName;

    if (mFile.is_open()) {
        while (std::getline(mFile, line)) {
            std::stringstream ss(line);
            std::string cmd; ss >> cmd;
            if (cmd == "newmtl") {
                ss >> currentMatName;
            }
            else if (cmd == "Kd") {
                float r, g, b;
                ss >> r >> g >> b;
                materials[currentMatName] = { r, g, b };
            }
        }
        mFile.close();
    }

    // 2. قراءة ملف OBJ
    std::ifstream oFile(objFile);
    if (!oFile.is_open()) {
        std::cerr << "Error: Could not open OBJ file: " << objFile << std::endl;
        return false;
    }

    vertices.clear();
    groups.clear();
    Group* currentGroup = nullptr;

    while (std::getline(oFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string cmd; ss >> cmd;

        if (cmd == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back(Point(x, y, z));
        }
        else if (cmd == "usemtl") {
            std::string mat;
            ss >> mat;
            groups.push_back(Group());
            currentGroup = &groups.back();
            currentGroup->materialName = mat;
        }
        else if (cmd == "f") {
            if (!currentGroup) {
                groups.push_back(Group());
                currentGroup = &groups.back();
                currentGroup->materialName = "default";
            }
            Face face;
            std::string vData;
            while (ss >> vData) {
                size_t slash = vData.find('/');
                int idx = std::stoi(vData.substr(0, slash));
                // التعامل مع الفهارس الموجبة والسالبة وتحويلها لـ 0-based
                face.vertexIndices.push_back(idx > 0 ? idx - 1 : (int)vertices.size() + idx);
            }
            currentGroup->faces.push_back(face);
        }
    }
    oFile.close();
    return true;
}

void ModelLoader::draw() {
    // تفعيل خاصية الشفافية (Blending)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // تعطيل الإضاءة لجعل الألوان حيوية وفاقعة (Emissive Look)
    glDisable(GL_LIGHTING);

    for (auto& group : groups) {
        float alpha = 1.0f; // القيمة الافتراضية (غير شفاف)

        // التحقق من اسم المادة للتعامل مع الزجاج
        std::string matName = group.materialName;
        std::transform(matName.begin(), matName.end(), matName.begin(), ::tolower);

        if (materials.count(group.materialName)) {
            Material m = materials[group.materialName];

            // إذا كان اسم المادة يحتوي على glass، نجعلها شفافة
            if (matName.find("glass") != std::string::npos || matName.find("window") != std::string::npos) {
                alpha = 0.3f; // درجة الشفافية (كلما قلت زاد الشفافية)
                glColor4f(m.r, m.g, m.b, alpha);
            }
            else {
                glColor4f(m.r, m.g, m.b, 1.0f);
            }
        }
        else {
            glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        }

        for (const auto& face : group.faces) {
            glBegin(GL_POLYGON);
            for (int idx : face.vertexIndices) {
                if (idx >= 0 && idx < (int)vertices.size()) {
                    glVertex3f(vertices[idx].x, vertices[idx].y, vertices[idx].z);
                }
            }
            glEnd();
        }
    }

    // إعادة الإعدادات كما كانت لكي لا يتأثر باقي المعرض
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}