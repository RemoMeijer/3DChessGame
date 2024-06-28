//
// Created by remo on 6/21/24.
//

#ifndef KNIGHT_H
#define KNIGHT_H
#include "Piece.h"

class Graphics;

class Knight : public Piece {
public:
    Knight(Graphics *graphics, glm::ivec2 pos, PieceColor);
    ~Knight();
};

#endif //KNIGHT_H
