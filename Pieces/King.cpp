//
// Created by remo on 6/21/24.
//

#include "King.h"

King::King(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::kingModel, pos, KING, pieceColor) { }

bool King::CheckIfCheck() {
    return false;
}

King::~King() {

}
