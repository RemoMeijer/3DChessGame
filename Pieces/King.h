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


    std::vector<glm::ivec2> getValidMoves(const std::vector<Piece*>& boardState) override;
    std::vector<glm::ivec2> getCaptureMoves(const std::vector<Piece*>& boardState) override;

    std::vector<glm::ivec2> kingDirections = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Horizontal and vertical moves
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonal moves
    };
};



#endif //KING_H
