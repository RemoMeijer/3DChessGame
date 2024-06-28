//
// Created by remo on 6/21/24.
//

#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"


class Rook : public Piece{
public:
    Rook(Graphics *graphics, glm::ivec2 pos, PieceColor);
    ~Rook();
};



#endif //ROOK_H
