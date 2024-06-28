//
// Created by remo on 6/20/24.
//

#include "Piece.h"

#include <iostream>
#include <glm/ext/matrix_transform.hpp>

// Initialize static members
ObjModel* Piece::pawnModel = nullptr;
ObjModel* Piece::rookModel = nullptr;
ObjModel* Piece::knightModel = nullptr;
ObjModel* Piece::bishopModel = nullptr;
ObjModel* Piece::queenModel = nullptr;
ObjModel* Piece::kingModel = nullptr;

constexpr float movingThreshold = 0.10f;    // Threshold of when piece is close enough to target
constexpr static float tileSpacing = 1.24f; // Space object needs to move for each tile
constexpr static float xStartPos = 2.95f;   // X Distance from side to middle of the tile
constexpr static float zStartPos = 0.68f;   // Z Distance from side to middle of the tile
constexpr float animationSpeed = 6.5f;      // Speed of animation, higher >> quicker animation

constexpr glm::vec4 blackPieceColor = glm::vec4(0.2078f, 0.2235, 0.2078f, 1);
constexpr glm::vec4 whitePieceColor = glm::vec4(1, 0.8921f, 0.6156f, 1);

Piece::Piece(Graphics* graphics, ObjModel* model, glm::ivec2 pos, glm::vec3 pieceScaling, PieceType type, PieceColor pieceCol) {
    gph = graphics;
    objModel = model;
    drawnColor = pieceCol == WHITE ? whitePieceColor : blackPieceColor;
    pieceType = type;
    pieceColor = pieceCol;
    moveToTarget = false;
    gridPosition = pos;
    position = gridToDrawPosition(gridPosition.x, gridPosition.y);
    scale = pieceScaling;
}

// Draw a piece
void Piece::draw() {
    Drawable::draw();
    objModel->setColor(drawnColor);
    objModel->draw();
}

// Convert grid cords to real float points for drawing on the board
glm::vec3 Piece::gridToDrawPosition(int x, int z) {
    return {xStartPos + x * tileSpacing, 0, zStartPos + z * tileSpacing};
}

void Piece::loadModels() {
    pawnModel = new ObjModel("Resources/Pawn/pawn.obj");
    rookModel = new ObjModel("Resources/Rook/rook.obj");
    knightModel = new ObjModel("Resources/Knight/knight.obj");
    bishopModel = new ObjModel("Resources/Bishop/bishop.obj");
    queenModel = new ObjModel("Resources/Queen/queen.obj");
    kingModel = new ObjModel("Resources/King/king.obj");

}

void Piece::moveTo(glm::ivec2 newPosition) {
    destination = newPosition;
    moveToTarget = true;
}


void Piece::remove() {}

void Piece::update(float deltaTime) {
    if (!moveToTarget) {
        return;
    }

    position += directionToTarget(gridToDrawPosition(destination.x, destination.y)) * deltaTime * animationSpeed;

    // Stop moving once we get close enough
    if (distanceToTarget(gridToDrawPosition(destination.x, destination.y))  < movingThreshold) {
        gridPosition = destination;
        moveToTarget = false;
    }
}

glm::vec3 Piece::directionToTarget(glm::vec3 target)
{
    // Calculate difference between position and target
    glm::vec3 diff = glm::vec3(
        target.x - position.x,
        target.y - position.y,
        target.z - position.z
    );

    // Normalize result and return
    return glm::normalize(diff);
}

float Piece::distanceToTarget(glm::vec3 target)
{
    // Calculate difference between position and target
    // No need for y since its a flat plane
    glm::vec2 diff = glm::vec2(
        position.x - target.x,
        position.z - target.z
    );

    // Calculate distance using euclidean formula
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    return distance;
}

void Piece::isPicked() {
    position.y = 0.5f;
}

void Piece::isNotPicked() {
    position.y = 0.0f;
}

void Piece::captured() {
    position = glm::vec3(100, 0, 100);
    gridPosition = glm::ivec2(100, 100);
    moveToTarget = false;
}

Piece::~Piece() { }
