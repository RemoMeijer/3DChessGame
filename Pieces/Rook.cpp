//
// Created by remo on 6/21/24.
//

#include "Rook.h"

Rook::Rook(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::rookModel, pos, glm::vec3(0.2f), ROOK, pieceColor) {
}

Rook::~Rook() { }
