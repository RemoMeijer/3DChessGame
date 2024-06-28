#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../BoardLogic.h"

class MockPiece : public Piece {

public:
    MOCK_METHOD(void, moveTo, (glm::ivec2 newPosition), (override));
    MockPiece(Graphics* graphics, ObjModel* model, glm::ivec2 pos,
              glm::vec3 pieceScaling, PieceType pieceType, PieceColor pieceColor)
        : Piece(graphics, model, pos, pieceScaling, pieceType, pieceColor) {}
};

class MockBoardLogic : public BoardLogic {
public:
    MOCK_METHOD(void, AddPiece, (Piece* piece), (override));
};

// Example test case
TEST(BoardLogicTest, AddPieceTest) {
    Graphics* mockGraphics = nullptr;
    ObjModel* mockModel = nullptr;
    glm::ivec2 mockPosition(0, 0);
    glm::vec3 mockScaling(1.0f);
    PieceType mockPieceType = PieceType::PAWN;
    PieceColor mockPieceColor = PieceColor::WHITE;

    MockBoardLogic board;
    MockPiece mockPiece(mockGraphics, mockModel, mockPosition,
                        mockScaling, mockPieceType, mockPieceColor);

    EXPECT_CALL(board, AddPiece(&mockPiece));

    board.AddPiece(&mockPiece);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
