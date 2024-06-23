//
// Created by remo on 6/21/24.
//

#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"


class Rook : public Piece{
public:
    Rook(Graphics *graphics, ObjModel *model, glm::vec4 col, glm::ivec2 pos, PieceType, PieceColor);
    ~Rook();
};



#endif //ROOK_H
