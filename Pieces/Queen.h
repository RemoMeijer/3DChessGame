//
// Created by remo on 6/21/24.
//

#ifndef QUEEN_H
#define QUEEN_H
#include "Piece.h"


class Queen : public Piece {
public:
    Queen(Graphics *graphics, glm::ivec2 pos, PieceColor);
    ~Queen();
};



#endif //QUEEN_H
