//
// Created by remo on 6/21/24.
//

#include "Knight.h"

Knight::Knight(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::knightModel, pos, glm::vec3(0.2f), KNIGHT, pieceColor) {
}

Knight::~Knight() {

}
