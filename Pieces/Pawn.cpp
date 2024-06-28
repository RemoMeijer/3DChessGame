//
// Created by remo on 6/19/24.
//

#include "Pawn.h"

Pawn::Pawn(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::pawnModel, pos, glm::vec3(0.2f), PAWN, pieceColor) {
}


Pawn::~Pawn() { }

