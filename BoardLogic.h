//
// Created by remo on 6/20/24.
//

#ifndef BOARDLOGIC_H
#define BOARDLOGIC_H

#include "Pieces/Piece.h"
#include <SFML/Audio.hpp>
#include <algorithm>
#include <iostream>



enum boardstates {SELECTING, MOVING};
constexpr int WHITEPAWNSTARTPOS = 6;
constexpr int BLACKPAWNSTARTPOS = 1;

class BoardLogic {
private:
    Piece* selectedPiece;

public:
    BoardLogic();
    void playSound(const std::string& filePath);
    bool isMoveValid(Piece* piece, glm::ivec2 position, glm::ivec2 newPosition);

    void AddPiece(Piece *piece);

    void checkMoves();
    void makeMove(int x, int y);
    void selectPiece(int x, int y);
    void changeTurn();

    bool checkDirection(Piece* checkedPiece, int x, int y);
    void checkPawnMoves(Piece* checkedPiece);
    void checkBishopMoves(Piece* piece);
    void checkKnightMoves(Piece* piece);
    void checkRookMoves(Piece* piece);
    void checkQueenMoves(Piece* piece);
    void checkKingMoves(Piece* piece);

    // Functions for getting capture moves only
    std::vector<glm::ivec2> potentialCaptureTiles; // Array for potential captures when checking for checks
    void getPawnCaptureMoves(Piece* checkedPiece);
    void getRookCaptureMoves(Piece* checkedPiece);
    void getQueenCaptureMoves(Piece* checkedPiece);
    void getBishopCaptureMoves(Piece* checkedPiece);
    void getKnightCaptureMoves(Piece* checkedPiece);
    void getKingCaptureMoves(Piece* checkedPiece);


    void selectRandomMove(Piece* piece);
    std::vector<Piece*> getBotPieces(PieceColor color);

    bool isKingInCheck(Piece* king);
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

    int BOARD_WIDTH = 8;
    int BOARD_HEIGHT = 8;

    sf::SoundBuffer buffer;
    sf::Sound sound;

    void makeBotMove();
};

#endif //BOARDLOGIC_H
