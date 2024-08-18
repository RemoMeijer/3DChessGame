//
// Created by remo on 6/21/24.
//

#include "Queen.h"

Queen::Queen(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::queenModel, pos, QUEEN, pieceColor) {
}

Queen::~Queen() {
}

std::vector<glm::ivec2> Queen::getValidMoves(const std::vector<Piece*>& boardState) {
    std::vector<glm::ivec2> validMoves;

    for (const auto& dir : queenDirections) {
        auto directionMoves = checkDirection(boardState, dir.x, dir.y);
        validMoves.insert(validMoves.end(), directionMoves.begin(), directionMoves.end());
    }

    return validMoves;
}

std::vector<glm::ivec2> Queen::getCaptureMoves(const std::vector<Piece*>& boardState) {
    std::vector<glm::ivec2> captureMoves;

    for (const auto& dir : queenDirections) {
        auto directionMoves = checkDirection(boardState, dir.x, dir.y, true);
        captureMoves.insert(captureMoves.end(), directionMoves.begin(), directionMoves.end());
    }

    return captureMoves;
}