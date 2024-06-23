//
// Created by remo on 6/19/24.
//

#include "Pawn.h"

Pawn::Pawn(Graphics *graphics, ObjModel *model, glm::vec4 col, glm::ivec2 pos, PieceType pieceType, PieceColor pieceColor)
    : Piece(graphics, model, col, pos, glm::vec3(0.2f), pieceType, pieceColor) {
}


Pawn::~Pawn() { }

