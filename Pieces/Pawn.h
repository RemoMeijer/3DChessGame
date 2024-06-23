//
// Created by remo on 6/19/24.
//

#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Graphics;

class Pawn : public Piece{
public:
    Pawn(Graphics *graphics, ObjModel *model, glm::vec4 col, glm::ivec2 pos, PieceType, PieceColor);
    ~Pawn();
};

#endif //PAWN_H
