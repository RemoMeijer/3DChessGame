//
// Created by remo on 6/19/24.
//

#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Graphics;

class Pawn : public Piece{
public:
    Pawn(Graphics *graphics, glm::ivec2 pos, PieceColor);
    ~Pawn();
};

#endif //PAWN_H
