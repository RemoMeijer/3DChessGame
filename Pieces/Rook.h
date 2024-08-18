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


    std::vector<glm::ivec2> getValidMoves(const std::vector<Piece*>& boardState) override;
    std::vector<glm::ivec2> getCaptureMoves(const std::vector<Piece*>& boardState) override;

    std::vector<glm::ivec2> rookDirections = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
};



#endif //ROOK_H
