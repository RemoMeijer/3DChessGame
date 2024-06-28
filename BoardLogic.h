//
// Created by remo on 6/20/24.
//

#ifndef BOARDLOGIC_H
#define BOARDLOGIC_H

#include "Pieces/Piece.h"
#include <SFML/Audio.hpp>
#include <algorithm>
#include <iostream>

#include "Pieces/Bishop.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"

enum boardstates {SELECTING, MOVING};
constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;

constexpr int WHITEPAWNSTARTPOS = 6;
constexpr int BLACKPAWNSTARTPOS = 1;


class BoardLogic {
    Piece* selectedPiece;

public:
    BoardLogic();
    void playSound(const std::string& filePath);
    virtual void AddPiece(Piece *piece);

    void makeMove(int x, int y);
    void selectPiece(int x, int y);
    virtual void changeTurn();

    bool checkDirection(Piece* checkedPiece, int x, int y);
    void checkPawnMoves(Pawn* checkedPiece);
    void checkBishopMoves(Bishop* piece);
    void checkKnightMoves(Knight* piece);
    void checkRookMoves(Rook* piece);
    void checkQueenMoves(Queen* piece);
    void checkKingMoves(King* piece);

    std::vector<glm::ivec2> potentialCaptureTiles;
    void getPawnCaptureMoves(Pawn* checkedPiece);
    void getRookCaptureMoves(Rook* checkedPiece);
    void getQueenCaptureMoves(Queen* checkedPiece);
    void getBishopCaptureMoves(Bishop* checkedPiece);
    void getKnightCaptureMoves(Knight* checkedPiece);
    void getKingCaptureMoves(King* checkedPiece);

    void selectRandomMove(Piece* piece);
    std::vector<Piece*> getBotPieces(PieceColor color);

    bool isKingInCheck(King* king);
    Piece* findKing(PieceColor pieceColor);
    bool isKingInCheckAfterMove(Piece* piece, const glm::ivec2& newPos);
    void filterLegalMoves(Piece* piece);


    std::vector<Piece*> boardState;
    std::vector<glm::ivec2> validTiles;
    std::vector<glm::ivec2> captureTiles;

    int currentState;
    bool turnChanged;
    bool whiteTurn;
    bool singlePlayer;
    bool gameEnded;

    sf::SoundBuffer buffer;
    sf::Sound sound;

    int botMoveCounter;
    void makeBotMove();
};

#endif //BOARDLOGIC_H
