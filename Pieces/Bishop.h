//
// Created by remo on 6/21/24.
//

#ifndef BISHOP_H
#define BISHOP_H
#include "Piece.h"


class Bishop : public Piece {
public:
    Bishop(Graphics *graphics, glm::ivec2 pos, PieceColor);
    ~Bishop();
};

#endif //BISHOP_H
