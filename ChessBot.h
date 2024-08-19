//
// Created by remo on 8/19/24.
//

#ifndef CHESSBOT_H
#define CHESSBOT_H

#include "Pieces/Piece.h"
#include <vector>
#include <set>
class BoardLogic;

class ChessBot {
public:
    ChessBot(PieceColor color, BoardLogic* boardLogic);

    void makeMove();

private:
    PieceColor botColor;
    BoardLogic* boardLogic;
    std::set<Piece*> attemptedPieces; // To track pieces that have been tried

    std::vector<Piece*> getBotPieces();
    void selectRandomMove(Piece* piece);
    int evaluateMove(Piece* piece, const glm::ivec2& move);
    void selectBestMove(Piece* piece);
    int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta);
};

#endif // CHESSBOT_H

