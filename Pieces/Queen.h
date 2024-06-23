//
// Created by remo on 6/21/24.
//

#ifndef QUEEN_H
#define QUEEN_H
#include "Piece.h"


class Queen : public Piece {
public:
    Queen(Graphics *graphics, ObjModel *model, glm::vec4 col, glm::ivec2 pos, PieceType, PieceColor);
    ~Queen();
};



#endif //QUEEN_H
