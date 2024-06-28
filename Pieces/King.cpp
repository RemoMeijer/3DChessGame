//
// Created by remo on 6/21/24.
//

#include "King.h"

King::King(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::kingModel, pos, glm::vec3(0.2f), KING, pieceColor) { }

bool King::CheckIfCheck() {
    return false;
}

King::~King() {

}
