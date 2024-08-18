//
// Created by remo on 6/21/24.
//

#ifndef KNIGHT_H
#define KNIGHT_H
#include "Piece.h"

class Graphics;

class Knight : public Piece {
public:
    Knight(Graphics *graphics, glm::ivec2 pos, PieceColor);
    ~Knight();

    std::vector<glm::ivec2> knightDirections = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    std::vector<glm::ivec2> getValidMoves(const std::vector<Piece*>& boardState) override;
    std::vector<glm::ivec2> getCaptureMoves(const std::vector<Piece*>& boardState) override;
};

#endif //KNIGHT_H
