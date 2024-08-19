//
// Created by remo on 8/19/24.
//

#include "ChessBot.h"
#include "BoardLogic.h"
#include <algorithm>
#include <cstdlib> // For std::rand
#include <map>

ChessBot::ChessBot(PieceColor color, BoardLogic *boardLogic)
    : botColor(color), boardLogic(boardLogic) {}

std::vector<Piece*> ChessBot::getBotPieces() {
    std::vector<Piece*> botPieces;
    for (Piece* piece : boardLogic->boardState) {
        if (piece->pieceColor == botColor) {
            botPieces.push_back(piece);
        }
    }
    return botPieces;
}

int ChessBot::evaluateMove(Piece* piece, const glm::ivec2& move) {
    int score = 0;

    // Piece values (basic heuristic)
    std::map<PieceType, int> pieceValues = {
        {PAWN, 1}, {KNIGHT, 3}, {BISHOP, 3}, {ROOK, 5}, {QUEEN, 9}, {KING, 1000}
    };

    // Check if the move is a capture
    for (Piece* otherPiece : boardLogic->boardState) {
        if (otherPiece->gridPosition == move && otherPiece->pieceColor != piece->pieceColor) {
            score += pieceValues[otherPiece->pieceType];
            break;
        }
    }

    // Encourage center control
    if (move == glm::ivec2(3, 3) || move == glm::ivec2(3, 4) ||
        move == glm::ivec2(4, 3) || move == glm::ivec2(4, 4)) {
        score += 2;
        }

    // Discourage moves that leave the piece vulnerable to capture
    for (Piece* otherPiece : boardLogic->boardState) {
        if (otherPiece->pieceColor != piece->pieceColor) {
            std::vector<glm::ivec2> opponentMoves = otherPiece->getValidMoves(boardLogic->boardState);
            if (std::find(opponentMoves.begin(), opponentMoves.end(), move) != opponentMoves.end()) {
                score -= pieceValues[piece->pieceType]; // Penalize if the piece can be captured
            }
        }
    }

    return score;
}

int ChessBot::minimax(int depth, bool isMaximizingPlayer, int alpha, int beta) {
    if (depth == 0 || boardLogic->gameEnded) {
        return 0; // You need to implement evaluateBoardState()
    }

    if (isMaximizingPlayer) {
        int maxEval = -9999;
        std::vector<Piece*> botPieces = getBotPieces();

        for (Piece* piece : botPieces) {
            std::vector<glm::ivec2> validMoves = piece->getValidMoves(boardLogic->boardState);
            std::vector<glm::ivec2> captureMoves = piece->getCaptureMoves(boardLogic->boardState);

            validMoves.insert(validMoves.end(), captureMoves.begin(), captureMoves.end());
            boardLogic->filterLegalMoves(piece);

            for (const auto& move : validMoves) {
                // Make the move (hypothetically)
                glm::ivec2 originalPos = piece->gridPosition;
                piece->gridPosition = move;

                int eval = minimax(depth - 1, false, alpha, beta);

                // Undo the move
                piece->gridPosition = originalPos;

                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break; // Beta cut-off
                }
            }
        }
        return maxEval;
    } else {
        int minEval = 9999;
        std::vector<Piece*> opponentPieces;
        for (Piece* otherPiece : boardLogic->boardState) {
            if (otherPiece->pieceColor != BLACK) {
                opponentPieces.push_back(otherPiece);
            }
        }

        for (Piece* piece : opponentPieces) {
            std::vector<glm::ivec2> validMoves = piece->getValidMoves(boardLogic->boardState);
            std::vector<glm::ivec2> captureMoves = piece->getCaptureMoves(boardLogic->boardState);

            validMoves.insert(validMoves.end(), captureMoves.begin(), captureMoves.end());
            boardLogic->filterLegalMoves(piece);

            for (const auto& move : validMoves) {
                // Make the move (hypothetically)
                glm::ivec2 originalPos = piece->gridPosition;
                piece->gridPosition = move;

                int eval = minimax(depth - 1, true, alpha, beta);

                // Undo the move
                piece->gridPosition = originalPos;

                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break; // Alpha cut-off
                }
            }
        }
        return minEval;
    }
}

// Select the best move based on the evaluation function
void ChessBot::selectBestMove(Piece* piece) {
    boardLogic->validTiles.clear();
    boardLogic->captureTiles.clear();

    boardLogic->selectedPiece = piece;

    std::vector<glm::ivec2> validMoves = piece->getValidMoves(boardLogic->boardState);
    std::vector<glm::ivec2> captureMoves = piece->getCaptureMoves(boardLogic->boardState);

    boardLogic->validTiles = validMoves;
    boardLogic->captureTiles = captureMoves;

    boardLogic->filterLegalMoves(piece);

    std::vector<glm::ivec2> allMoves = boardLogic->validTiles;
    allMoves.insert(allMoves.end(), boardLogic->captureTiles.begin(), boardLogic->captureTiles.end());

    if (!allMoves.empty()) {
        int bestScore = -1;
        glm::ivec2 bestMove;

        for (const auto& move : allMoves) {
            int score = evaluateMove(piece, move);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }

        boardLogic->makeMove(bestMove.x, bestMove.y);
        attemptedPieces.clear(); // Reset attempted pieces if a move was made
    } else {
        attemptedPieces.insert(piece); // Mark this piece as attempted
        makeMove(); // Try another move
    }
}

void ChessBot::makeMove() {
    if (boardLogic->gameEnded) return;

    int bestScore = -9999;
    glm::ivec2 bestMove;
    Piece* bestPiece = nullptr;

    std::vector<Piece*> botPieces = getBotPieces();

    for (Piece* piece : botPieces) {
        std::vector<glm::ivec2> validMoves = piece->getValidMoves(boardLogic->boardState);
        std::vector<glm::ivec2> captureMoves = piece->getCaptureMoves(boardLogic->boardState);

        validMoves.insert(validMoves.end(), captureMoves.begin(), captureMoves.end());
        boardLogic->filterLegalMoves(piece);

        for (const auto& move : validMoves) {
            // Make the move (hypothetically)
            glm::ivec2 originalPos = piece->gridPosition;
            piece->gridPosition = move;

            int moveScore = minimax(3, false, -10000, 10000); // Set depth to 3 or more

            // Undo the move
            piece->gridPosition = originalPos;

            if (moveScore > bestScore) {
                bestScore = moveScore;
                bestMove = move;
                bestPiece = piece;
            }
        }
    }

    if (bestPiece != nullptr) {
        boardLogic->validTiles.clear();
        boardLogic->captureTiles.clear();
        boardLogic->selectedPiece = bestPiece;

        std::vector<glm::ivec2> validMoves = bestPiece->getValidMoves(boardLogic->boardState);
        std::vector<glm::ivec2> captureMoves = bestPiece->getCaptureMoves(boardLogic->boardState);

        boardLogic->validTiles = validMoves;
        boardLogic->captureTiles = captureMoves;

        boardLogic->filterLegalMoves(bestPiece);

        boardLogic->makeMove(bestMove.x, bestMove.y);
    } else {
        std::cout << "No legal bot moves available. Checkmate or stalemate." << std::endl;
        boardLogic->gameEnded = true;
    }
}