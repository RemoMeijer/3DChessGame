//
// Created by remo on 6/21/24.
//

#include "Rook.h"

Rook::Rook(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::rookModel, pos, ROOK, pieceColor) {
}

Rook::~Rook() { }

std::vector<glm::ivec2> Rook::getValidMoves(const std::vector<Piece*>& boardState) {
    std::vector<glm::ivec2> validMoves;

    for (const auto& dir : rookDirections) {
        auto directionMoves = checkDirection(boardState, dir.x, dir.y);
        validMoves.insert(validMoves.end(), directionMoves.begin(), directionMoves.end());
    }

    return validMoves;
}

std::vector<glm::ivec2> Rook::getCaptureMoves(const std::vector<Piece*>& boardState) {
    std::vector<glm::ivec2> captureMoves;

    for (const auto& dir : rookDirections) {
        auto directionMoves = checkDirection(boardState, dir.x, dir.y, true);
        captureMoves.insert(captureMoves.end(), directionMoves.begin(), directionMoves.end());
    }

    return captureMoves;
}