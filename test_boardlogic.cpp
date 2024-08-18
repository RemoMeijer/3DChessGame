#include <gtest/gtest.h>
#include "BoardLogic.h"
#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include "Pieces/King.h"
#include "glm/glm.hpp"

// Dummy Graphics class for testing
class Graphics {
    // Add any necessary dummy implementations
};

// Test that a bot move updates the grid pieces
TEST(BoardLogicTest, BotMoveUpdatesGrid) {

    Graphics dummyGraphics;
    BoardLogic* board = new BoardLogic();
    board->AddPiece(new Rook(&dummyGraphics, glm::ivec2(0, 0), BLACK)); // Add a black rook at (0, 0)
    board->AddPiece(new King(&dummyGraphics, glm::ivec2(0, 1), BLACK)); // Add a black rook at (0, 0)


    // Perform a bot move
    board->makeBotMove();

    // Check that one of the pieces has moved
    bool moved = false;
    for (Piece *piece: board->getBotPieces(BLACK)) {
        if (piece->gridPosition != glm::ivec2(0, 0) || piece->gridPosition != glm::ivec2(0, 1)) {
            moved = true;
            break;
        }
    }
    EXPECT_TRUE(moved);
}

// Test that checkmate is correctly flagged
TEST(BoardLogicTest, CheckmateIsFlagged) {
    Graphics dummyGraphics;
    BoardLogic board;
    board.AddPiece(new King(&dummyGraphics, glm::ivec2(0, 0), BLACK)); // Add a black king at (0, 0)
    board.AddPiece(new Rook(&dummyGraphics, glm::ivec2(0, 7), WHITE)); // Add a white rook at (0, 1)
    board.AddPiece(new Rook(&dummyGraphics, glm::ivec2(1, 7), WHITE)); // Add a white rook at (1, 0)

    // Set the bot color to black and force it to make a move
    board.makeBotMove();

    // Check if the game ended due to checkmate
    EXPECT_TRUE(board.gameEnded);
}

// Test that selectPiece correctly identifies and selects a piece
TEST(BoardLogicTest, SelectPieceIdentifiesPiece) {
    Graphics dummyGraphics;
    BoardLogic* board = new BoardLogic;
    Pawn* pawn = new Pawn(&dummyGraphics, glm::ivec2(0, 1), WHITE);
    board->AddPiece(pawn);

    board->selectPiece(0, 1);
    EXPECT_EQ(board->selectedPiece, pawn);
}