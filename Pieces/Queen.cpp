//
// Created by remo on 6/21/24.
//

#include "Queen.h"

Queen::Queen(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::queenModel, pos, glm::vec3(0.2f), QUEEN, pieceColor) {
}

Queen::~Queen() {
}
