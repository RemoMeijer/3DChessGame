//
// Created by remo on 6/21/24.
//

#ifndef KING_H
#define KING_H
#include "Piece.h"


class King : public Piece {
public:
    King(Graphics *graphics, glm::ivec2 pos, PieceColor);
    bool CheckIfCheck();
    ~King();
};



#endif //KING_H
