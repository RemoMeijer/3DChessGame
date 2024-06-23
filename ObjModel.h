//
// Created by remo on 6/19/24.
//

#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <list>

#include "tigl.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Texture.h"

class Texture;

class ObjModel {
private:
    class Vertex {
    public:
        int position;
        int normal;
        int texcoord;
    };

    class Face {
    public:
        std::list<Vertex> vertices;
    };

    class MaterialInfo {
    public:
        MaterialInfo();

        std::string name;
        Texture *texture;
        glm::vec4 color{};
    };

    class ObjGroup {
    public:
        std::string name;
        int materialIndex;
        std::list<Face> faces;
    };

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<ObjGroup *> groups;
    std::vector<MaterialInfo *> materials;

    tigl::VBO* vbo;

    void loadMaterialFile(const std::string &fileName, const std::string &dirName);

public:
    ObjModel(const std::string &filename);

    ~ObjModel(void);

    void init();

    void setColor(glm::vec4);

    void draw();
};

#endif //OBJMODEL_H
