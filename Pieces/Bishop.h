//
// Created by remo on 6/21/24.
//

#ifndef BISHOP_H
#define BISHOP_H
#include "Piece.h"


class Bishop : public Piece {
public:
    Bishop(Graphics *graphics, ObjModel *model, glm::vec4 col, glm::ivec2 pos, PieceType, PieceColor);
    ~Bishop();
};

#endif //BISHOP_H
