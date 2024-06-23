//
// Created by remo on 6/22/24.
//

#include "Tile.h"

Tile::Tile(float x, float y, float size)
    : position(x, 0, y), size(size), isHighlighted(false) {
    // Create vertices for the quad
    float halfSize = size;
    vertices.push_back(tigl::Vertex::PC(glm::vec3(-halfSize + x, 0.001f, halfSize + y), glm::vec4(0, 1, 0, 0.5)));
    vertices.push_back(tigl::Vertex::PC(glm::vec3(-halfSize + x, 0.001f, -halfSize + y), glm::vec4(0, 1, 0, 0.5)));
    vertices.push_back(tigl::Vertex::PC(glm::vec3(halfSize + x, 0.001f, -halfSize + y), glm::vec4(0, 1, 0, 0.5)));
    vertices.push_back(tigl::Vertex::PC(glm::vec3(halfSize + x, 0.001f, halfSize + y), glm::vec4(0, 1, 0, 0.5)));
}

void Tile::draw() {
    // Change color based on highlight state
    glm::vec4 color = isHighlighted ? glm::vec4(1, 0, 0, 0.5) : glm::vec4(0, 1, 0, 0.0f);
    for (auto& vertex : vertices) {
        vertex.color = color;
    }
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
    tigl::shader->setModelMatrix(modelMatrix);

    tigl::drawVertices(GL_QUADS, vertices);
}

void Tile::setHighlight(bool highlight) {
    isHighlighted = highlight;
}