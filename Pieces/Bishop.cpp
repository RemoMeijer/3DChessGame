//
// Created by remo on 6/21/24.
//

#include "Bishop.h"

Bishop::Bishop(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::bishopModel, pos, BISHOP, pieceColor) {
}

Bishop::~Bishop() {
}

std::vector<glm::ivec2> Bishop::getValidMoves(const std::vector<Piece*>& boardState) {
    std::vector<glm::ivec2> validMoves;

    for (const auto& dir : bishopDirections) {
        auto directionMoves = checkDirection(boardState, dir.x, dir.y);
        validMoves.insert(validMoves.end(), directionMoves.begin(), directionMoves.end());
    }

    return validMoves;
}

std::vector<glm::ivec2> Bishop::getCaptureMoves(const std::vector<Piece*>& boardState) {
    std::vector<glm::ivec2> captureMoves;

    for (const auto& dir : bishopDirections) {
        auto directionMoves = checkDirection(boardState, dir.x, dir.y, true);
        captureMoves.insert(captureMoves.end(), directionMoves.begin(), directionMoves.end());
    }

    return captureMoves;
}
