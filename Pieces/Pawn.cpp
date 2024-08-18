//
// Created by remo on 6/19/24.
//

#include "Pawn.h"

Pawn::Pawn(Graphics *graphics, glm::ivec2 pos, PieceColor pieceColor)
    : Piece(graphics, Piece::pawnModel, pos, PAWN, pieceColor) {
}


Pawn::~Pawn() { }

std::vector<glm::ivec2> Pawn::getValidMoves(const std::vector<Piece*>& boardState) {
    std::vector<glm::ivec2> validMoves;
    int direction = (pieceColor == BLACK) ? 1 : -1;

    glm::ivec2 forward = gridPosition + glm::ivec2(0, direction);
    glm::ivec2 doubleForward = gridPosition + glm::ivec2(0, 2 * direction);

    // Check if the forward position is empty
    auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece* piece) {
        return piece->gridPosition == forward;
    });

    if (it == boardState.end()) {
        validMoves.push_back(forward);

        // Check for double move on the first move
        if ((pieceColor == BLACK && gridPosition.y == 1) || (pieceColor == WHITE && gridPosition.y == 6)) {
            it = std::find_if(boardState.begin(), boardState.end(), [&](Piece* piece) {
                return piece->gridPosition == doubleForward;
            });

            if (it == boardState.end()) {
                validMoves.push_back(doubleForward);
            }
        }
    }

    return validMoves;
}

std::vector<glm::ivec2> Pawn::getCaptureMoves(const std::vector<Piece*>& boardState) {
    std::vector<glm::ivec2> captureMoves;
    int direction = (pieceColor == BLACK) ? 1 : -1;

    glm::ivec2 captureLeft = gridPosition + glm::ivec2(-1, direction);
    glm::ivec2 captureRight = gridPosition + glm::ivec2(1, direction);

    auto addCaptureMove = [&](const glm::ivec2& capturePos) {
        auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece* piece) {
            return piece->gridPosition == capturePos && piece->pieceColor != pieceColor;
        });

        if (it != boardState.end()) {
            captureMoves.push_back(capturePos);
        }
    };

    addCaptureMove(captureLeft);
    addCaptureMove(captureRight);

    return captureMoves;
}

