//
// Created by remo on 6/21/24.
//

#include "King.h"

King::King(Graphics *graphics, ObjModel *model, glm::vec4 col, glm::ivec2 pos, PieceType pieceType, PieceColor pieceColor)
    : Piece(graphics, model, col, pos, glm::vec3(0.2f), pieceType, pieceColor) { }

bool King::CheckIfCheck() {
    return false;
}

King::~King() {

}
