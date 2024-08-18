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

    std::vector<glm::ivec2> getValidMoves(const std::vector<Piece*>& boardState) override;
    std::vector<glm::ivec2> getCaptureMoves(const std::vector<Piece*>& boardState) override;

    std::vector<glm::ivec2> bishopDirections = {
        {1, 1}, {-1, 1}, {1, -1}, {-1, -1}
    };
};

#endif //BISHOP_H
