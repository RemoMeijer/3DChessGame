//
// Created by remo on 6/21/24.
//

#ifndef KING_H
#define KING_H
#include "Piece.h"


class King : public Piece {
public:
    King(Graphics *graphics, ObjModel *model, glm::vec4 col, glm::ivec2 pos, PieceType, PieceColor);
    bool CheckIfCheck();
    ~King();
};



#endif //KING_H
