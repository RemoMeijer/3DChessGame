//
// Created by remo on 6/21/24.
//

#ifndef KNIGHT_H
#define KNIGHT_H
#include "Piece.h"

class Graphics;

class Knight : public Piece {
public:
    Knight(Graphics *graphics, ObjModel *model, glm::vec4 col, glm::ivec2 pos, PieceType, PieceColor);
    ~Knight();
};

#endif //KNIGHT_H
