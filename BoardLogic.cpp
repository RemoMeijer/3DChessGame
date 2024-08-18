//
// Created by remo on 6/20/24.
//

#include "BoardLogic.h"

BoardLogic::BoardLogic() {
    singlePlayer = false; // Bot on or off -> todo make clickable in game
    whiteTurn = true; // White to start
    turnChanged = false; // Switch camera, or make the bot move
    selectedPiece = nullptr; // Piece that is picked-up
    currentState = SELECTING; // Start the game in the selecting state
    srand(time(nullptr)); // Seeding for some random moves
    botMoveCounter = 0;
    gameEnded = false;
}

void BoardLogic::AddPiece(Piece *piece) {
    boardState.push_back(piece);
}

void BoardLogic::changeTurn() {
    whiteTurn = !whiteTurn;
    turnChanged = true;
}

void BoardLogic::playSound(const std::string &filePath) {
    if (!buffer.loadFromFile(filePath)) {
        std::cout << "Failed to load sound!" << std::endl;
        return; // No need to crash, just skip playing sound
    }

    sound.setBuffer(buffer);
    sound.play();
}

// Move selected piece to destination click
void BoardLogic::makeMove(int x, int y) {
    // We make a legal move, or are prevented from moving, back to selecting
    currentState = SELECTING;

    // Check nullptr
    if (selectedPiece == nullptr) {
        std::cout << "Piece selection error!" << std::endl;
        return;
    }

    // Check if any moves available, just return when non are.
    if (captureTiles.empty() && validTiles.empty()) {
        std::cout << "No legal moves!" << std::endl;
        selectedPiece->isNotPicked();
        return;
    }

    // Check if the destination is a capture move first
    auto it = std::find_if(captureTiles.begin(), captureTiles.end(), [&](const glm::ivec2 &tile) {
        return tile.x == x && tile.y == y;
    });

    // Check if capture
    bool capture = false;
    if (it != captureTiles.end()) {
        // Find and move the captured piece
        for (Piece *piece: boardState) {
            if (piece->gridPosition == glm::ivec2(x, y)) {
                playSound("Resources/Sounds/capture.wav");
                piece->captured(); // Move captured piece to the shadow realm
                capture = true;
                break;
            }
        }
    }

    // Check if the destination is a valid move
    it = std::find_if(validTiles.begin(), validTiles.end(), [&](const glm::ivec2 &tile) {
        return tile.x == x && tile.y == y;
    });

    if (it != validTiles.end() || std::find_if(captureTiles.begin(), captureTiles.end(), [&](const glm::ivec2 &tile) {
        return tile.x == x && tile.y == y;
    }) != captureTiles.end()) {
        selectedPiece->moveTo(glm::ivec2(x, y));

        // Check if capture, to prevent playing two sound trough each other
        if (!capture) {
            playSound("Resources/Sounds/move-self.wav");
        }

        // Piece moved, other's turn!
        changeTurn();
        return;
    }

    // De-select piece if no move made
    selectedPiece->isNotPicked();
}

// Check selected piece moves on click
void BoardLogic::selectPiece(int x, int y) {
    for (Piece *piece: boardState) {
        if (piece->gridPosition == glm::ivec2(x, y)) {
            selectedPiece = piece;

            // Check if piece is the right color
            if ((selectedPiece->pieceColor == WHITE && !whiteTurn) || (
                    selectedPiece->pieceColor == BLACK && whiteTurn)) {
                std::cout << "Not your piece!" << std::endl;
                selectedPiece = nullptr;
                return;
            }

            // Clear the previous moves
            validTiles.clear();
            captureTiles.clear();

            // Pick the piece
            piece->isPicked();
            currentState = MOVING;

            // Get the valid and capture moves
            validTiles = piece->getValidMoves(boardState);
            captureTiles = piece->getCaptureMoves(boardState);

            // Filter moves that put own king in check
            filterLegalMoves(piece);
        }
    }
}


Piece *BoardLogic::findKing(PieceColor pieceColor) {
    for (Piece *piece: boardState) {
        if (piece->pieceType == KING && piece->pieceColor == pieceColor) {
            return piece;
        }
    }
    return nullptr;
}


bool BoardLogic::isKingInCheck(King *king) {
    for (Piece *piece: boardState) {
        if (piece->pieceColor != king->pieceColor) {
            potentialCaptureTiles = piece->getCaptureMoves(boardState);

            // If the king is one of them, it's in check
            for (const auto &tile: potentialCaptureTiles) {
                if (tile == king->gridPosition) {
                    return true;
                }
            }
        }
    }
    return false;
}

void BoardLogic::filterLegalMoves(Piece *piece) {
    // Remove valid tiles that will discover check own king after moving
    validTiles.erase(
        std::remove_if(validTiles.begin(), validTiles.end(),
                       [&](const glm::ivec2 &tile) {
                           return isKingInCheckAfterMove(piece, tile);
                       }),
        validTiles.end()
    );

    // Remove capture tiles that will discover check own king after capturing
    captureTiles.erase(
        std::remove_if(captureTiles.begin(), captureTiles.end(),
                       [&](const glm::ivec2 &tile) {
                           return isKingInCheckAfterMove(piece, tile);
                       }),
        captureTiles.end()
    );
}

// Check if the king is in check after a piece has moved
bool BoardLogic::isKingInCheckAfterMove(Piece *piece, const glm::ivec2 &newPos) {
    // Save the original position
    glm::ivec2 originalPos = piece->gridPosition;

    // Temporarily move the piece
    piece->gridPosition = newPos;

    // Temporarily remove the captured piece if present
    Piece *capturedPiece = nullptr;
    auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece *p) {
        return p->gridPosition == newPos && p->pieceColor != piece->pieceColor;
    });
    if (it != boardState.end()) {
        capturedPiece = *it;
        boardState.erase(it);
    }

    // Check if the king is in check after the move
    Piece *king = findKing(piece->pieceColor);
    if (king == nullptr) {
        std::cout << "ERROR, King not found!?" << std::endl;
        return false;
    }
    bool inCheck = isKingInCheck(dynamic_cast<King *>(king));

    // Move the piece back to its original position
    piece->gridPosition = originalPos;

    // Restore the captured piece if it was removed
    if (capturedPiece) {
        boardState.push_back(capturedPiece);
    }

    return inCheck;
}

std::vector<Piece *> BoardLogic::getBotPieces(PieceColor color) {
    std::vector<Piece *> botPieces;
    for (Piece *piece: boardState) {
        if (piece->pieceColor == color) {
            botPieces.push_back(piece);
        }
    }
    return botPieces;
}

void BoardLogic::selectRandomMove(Piece *piece) {
    // Clear the valid and capture tiles
    validTiles.clear();
    captureTiles.clear();

    selectedPiece = piece;

    piece->getValidMoves(boardState);

    // Filter out illegal moves
    filterLegalMoves(selectedPiece);

    // Combine valid moves and capture moves
    std::vector<glm::ivec2> allMoves = validTiles;
    allMoves.insert(allMoves.end(), captureTiles.begin(), captureTiles.end());

    if (!allMoves.empty()) {
        // Select a random move
        int randomIndex = std::rand() % allMoves.size();
        glm::ivec2 move = allMoves[randomIndex];
        makeMove(move.x, move.y);
        attemptedPieces.clear(); // Reset attempted pieces if a move was made
    } else {
        attemptedPieces.insert(piece); // Mark this piece as attempted
        makeBotMove();
    }
}

void BoardLogic::makeBotMove() {
    if (!gameEnded) {
        PieceColor botColor = BLACK; // Change to WHITE if you want the bot to play as white
        std::vector<Piece *> botPieces = getBotPieces(botColor);

        // Remove pieces that have already been attempted
        botPieces.erase(std::remove_if(botPieces.begin(), botPieces.end(),
                                       [this](Piece *piece) {
                                           return attemptedPieces.find(piece) != attemptedPieces.end();
                                       }),
                        botPieces.end());


        if (!botPieces.empty()) {
            // Select a random piece from remaining ones
            int randomIndex = std::rand() % botPieces.size();
            Piece *randomPiece = botPieces[randomIndex];
            selectRandomMove(randomPiece);
        } else {
            std::cout << "No legal bot moves available. Checkmate or stalemate." << std::endl;
            gameEnded = true;
        }
    }
}

BoardLogic::~BoardLogic() {
    for (Piece *piece: boardState) {
        delete piece;
    }
}
