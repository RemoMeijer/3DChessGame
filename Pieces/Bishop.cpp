//
// Created by remo on 6/21/24.
//

#include "Bishop.h"

Bishop::Bishop(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::bishopModel, pos, BISHOP, pieceColor) {
}

Bishop::~Bishop() {
}
