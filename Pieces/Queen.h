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


    std::vector<glm::ivec2> getValidMoves(const std::vector<Piece*>& boardState) override;
    std::vector<glm::ivec2> getCaptureMoves(const std::vector<Piece*>& boardState) override;

    std::vector<glm::ivec2> queenDirections = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {-1, 1}, {1, -1}, {-1, -1}
    };

};



#endif //QUEEN_H
