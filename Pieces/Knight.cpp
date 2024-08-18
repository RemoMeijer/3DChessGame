//
// Created by remo on 6/21/24.
//

#include "Knight.h"

Knight::Knight(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::knightModel, pos, KNIGHT, pieceColor) {
}

Knight::~Knight() {

}

std::vector<glm::ivec2> Knight::getValidMoves(const std::vector<Piece *> &boardState) {
    std::vector<glm::ivec2> validMoves;

    for (const auto& move : knightDirections) {
        glm::ivec2 newPos = gridPosition + move;

        // Check if the move is within the board bounds
        if (newPos.x >= 0 && newPos.x < 8 && newPos.y >= 0 && newPos.y < 8) {
            // Check if the tile is occupied by any piece
            auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece* piece) {
                return piece->gridPosition == newPos;
            });

            if (it == boardState.end()) {
                validMoves.push_back(newPos);  // The tile is empty
            }
        }
    }

    return validMoves;
}

std::vector<glm::ivec2> Knight::getCaptureMoves(const std::vector<Piece *> &boardState) {
    std::vector<glm::ivec2> captureMoves;

    for (const auto& move : knightDirections) {
        glm::ivec2 newPos = gridPosition + move;

        // Check if the move is within the board bounds
        if (newPos.x >= 0 && newPos.x < 8 && newPos.y >= 0 && newPos.y < 8) {
            // Check if the tile is occupied by an enemy piece
            auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece* piece) {
                return piece->gridPosition == newPos && piece->pieceColor != pieceColor;
            });

            if (it != boardState.end()) {
                captureMoves.push_back(newPos);  // The tile is occupied by an enemy piece
            }
        }
    }

    return captureMoves;
}
