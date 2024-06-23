//
// Created by remo on 6/22/24.
//

#ifndef TILE_H
#define TILE_H
#include <glm/glm.hpp>

#include "tigl.h"


class Tile {
public:
    Tile(float x, float y, float size);
    void draw();
    void setHighlight(bool highlight);

private:
    glm::vec3 position;
    float size;
    bool isHighlighted;
    std::vector<tigl::Vertex> vertices;
};




#endif //TILE_H
