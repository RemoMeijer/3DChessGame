//
// Created by remo on 6/20/24.
//

#ifndef PIECE_H
#define PIECE_H

#include <glm/glm.hpp>
#include "../Drawable.h"
#include "../ObjModel.h"

class Graphics;

enum PieceType { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
enum PieceColor { BLACK, WHITE };

class Piece : public Drawable {

public:
    ObjModel* objModel;
    glm::vec4 drawnColor{};
    Graphics* gph;
    PieceType pieceType;
    PieceColor pieceColor;
    glm::ivec2 gridPosition{};
    glm::ivec2 destination{};
    bool moveToTarget;

    Piece(Graphics* graphics, ObjModel* model, glm::vec4 drawnCol, glm::ivec2 pos, glm::vec3 pieceScaling
        , PieceType pieceType, PieceColor pieceColor);

    void remove();
    void update(float deltaTime) override;
    void draw() override;
    void captured();
    void isPicked();
    void isNotPicked();

    static glm::vec3 gridToDrawPosition(int x, int z);

    void moveTo(glm::ivec2 newPosition);
    glm::vec3 directionToTarget(glm::vec3 target);
    float distanceToTarget(glm::vec3 target);

    virtual ~Piece();
};

#endif
