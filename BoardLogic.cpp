//
// Created by remo on 6/20/24.
//

#include "BoardLogic.h"


BoardLogic::BoardLogic() {
    singlePlayer = false;
    whiteTurn = true; // White to start
    turnChanged = false;
    selectedPiece = nullptr;
    currentState = SELECTING; // Start the game in the selecting state
    srand(time(nullptr));
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
        return;
    }

    sound.setBuffer(buffer);
    sound.play();
}


bool BoardLogic::checkDirection(Piece *checkedPiece, int dx, int dy) {
    int x = checkedPiece->gridPosition.x + dx;
    int y = checkedPiece->gridPosition.y + dy;

    while (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
        // Check if the tile is occupied by an enemy piece
        auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece *piece) {
            return piece->gridPosition == glm::ivec2(x, y);
        });

        if (it == boardState.end()) {
            // Tile is empty
            validTiles.emplace_back(x, y);
        } else if ((*it)->pieceColor != checkedPiece->pieceColor) {
            // Tile is occupied by an enemy piece
            captureTiles.emplace_back(x, y);
            return true; // Stop further movement in this direction
        } else {
            // Tile is occupied by a friendly piece
            return true; // Stop further movement in this direction
        }

        x += dx;
        y += dy;
    }

    return false; // Continue movement in this direction
}

// Move selected piece to destination click
void BoardLogic::makeMove(int x, int y) {
    // We make an legal move, or are prevented from moving, back to selecting
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


    bool capture = false;
    if (it != captureTiles.end()) {
        // Find and move the captured piece
        for (Piece *piece: boardState) {
            if (piece->gridPosition == glm::ivec2(x, y)) {
                playSound("/home/remo/CLionProjects/3DGraphics/eindopdracht/Resources/Sounds/capture.wav");
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

        if(!capture) {
            playSound("/home/remo/CLionProjects/3DGraphics/eindopdracht/Resources/Sounds/move-self.wav");
        }

        // Piece moved, other's turn!
        changeTurn();
        return;
    }
    selectedPiece->isNotPicked();
}

// Check selected piece moves on click
void BoardLogic::selectPiece(int x, int y) {
    for (Piece *piece: boardState) {
        if (piece->gridPosition == glm::ivec2(x, y)) {
            selectedPiece = piece;

            // Check if piece is right color
            if (selectedPiece->pieceColor == 0 && whiteTurn || selectedPiece->pieceColor == 1 && !whiteTurn) {
                std::cout << "Not yer piece" << std::endl;
                return;
            }
            validTiles.clear();
            captureTiles.clear();

            piece->isPicked();

            // We be movin now
            currentState = MOVING;

            switch (piece->pieceType) {
                case PAWN:
                    checkPawnMoves(piece);
                    break;
                case ROOK: checkRookMoves(piece);
                    break;
                case QUEEN: checkQueenMoves(piece);
                    break;
                case BISHOP: checkBishopMoves(piece);
                    break;
                case KNIGHT: checkKnightMoves(piece);
                    break;
                case KING: checkKingMoves(piece);
                    break;
                default:
                    std::cout << "Invalid piece found." << std::endl;
                    break;
            }
            filterLegalMoves(selectedPiece);
        }
    }
}

void BoardLogic::checkPawnMoves(Piece *checkedPiece) {
    auto addMoveIfValid = [&](int x, int y) {
        // Check if the tile is not occupied by any piece
        if (std::none_of(boardState.begin(), boardState.end(), [&](Piece *piece) {
            return piece->gridPosition == glm::ivec2(x, y);
        })) {
            validTiles.emplace_back(x, y);
        }
    };

    auto addCaptureIfValid = [&](int x, int y) {
        // Check if the tile is occupied by an enemy piece
        if (std::any_of(boardState.begin(), boardState.end(), [&](Piece *piece) {
            return piece->gridPosition == glm::ivec2(x, y) && piece->pieceColor != checkedPiece->pieceColor;
        })) {
            captureTiles.emplace_back(x, y);
        }
    };

    // Black and white pawns move to opposite direction
    int direction = (checkedPiece->pieceColor == BLACK) ? 1 : -1;

    // Get the right starting position of pawn
    int startPos = (checkedPiece->pieceColor == BLACK) ? BLACKPAWNSTARTPOS : WHITEPAWNSTARTPOS;

    // Add normal move
    addMoveIfValid(checkedPiece->gridPosition.x, checkedPiece->gridPosition.y + direction);

    // Check if first double move is available
    if (checkedPiece->gridPosition.y == startPos) {
        addMoveIfValid(checkedPiece->gridPosition.x, checkedPiece->gridPosition.y + 2 * direction);
    }

    // Add capture moves
    addCaptureIfValid(checkedPiece->gridPosition.x - 1, checkedPiece->gridPosition.y + direction);
    addCaptureIfValid(checkedPiece->gridPosition.x + 1, checkedPiece->gridPosition.y + direction);

    // Remove valid tiles behind enemy piece
    if (checkedPiece->gridPosition.y == startPos) {
        bool hasFirstMove = std::any_of(validTiles.begin(), validTiles.end(), [&](const glm::ivec2 &tile) {
            return tile.y == checkedPiece->gridPosition.y + direction;
        });
        if (!hasFirstMove) {
            validTiles.clear();
        }
    }
}

// Check valid pawn moves
void BoardLogic::checkBishopMoves(Piece *checkedPiece) {
    validTiles.clear();
    captureTiles.clear();

    // Check all four diagonal directions for bishop movement
    checkDirection(checkedPiece, 1, 1); // Up-Right
    checkDirection(checkedPiece, -1, 1); // Up-Left
    checkDirection(checkedPiece, 1, -1); // Down-Right
    checkDirection(checkedPiece, -1, -1); // Down-Left
}

void BoardLogic::checkKnightMoves(Piece *checkedPiece) {
    validTiles.clear(); // Empty out valid tiles
    captureTiles.clear(); // Empty out capture tiles

    auto addMoveOrCapture = [&](int x, int y) {
        // Check if the tile is within the board bounds
        if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
            // Check if the tile is occupied by an enemy piece
            auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece *piece) {
                return piece->gridPosition == glm::ivec2(x, y);
            });

            if (it == boardState.end()) {
                // Tile is empty
                validTiles.emplace_back(x, y);
            } else if ((*it)->pieceColor != checkedPiece->pieceColor) {
                // Tile is occupied by an enemy piece
                captureTiles.emplace_back(x, y);
            }
        }
    };

    // List all possible moves for a knight
    std::vector<glm::ivec2> knightMoves = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (const auto &move: knightMoves) {
        addMoveOrCapture(checkedPiece->gridPosition.x + move.x, checkedPiece->gridPosition.y + move.y);
    }
}

void  BoardLogic::checkRookMoves(Piece *checkedPiece) {
    validTiles.clear(); // Empty out valid tiles
    captureTiles.clear(); // Empty out capture tiles

    // Check all four directions for rook movement
    checkDirection(checkedPiece, 1, 0); // Right
    checkDirection(checkedPiece, -1, 0); // Left
    checkDirection(checkedPiece, 0, 1); // Up
    checkDirection(checkedPiece, 0, -1); // Down
}

void  BoardLogic::checkQueenMoves(Piece *checkedPiece) {
    validTiles.clear(); // Empty out valid tiles
    captureTiles.clear(); // Empty out capture tiles

    // Check all eight directions for queen movement
    checkDirection(checkedPiece, 1, 0); // Right
    checkDirection(checkedPiece, -1, 0); // Left
    checkDirection(checkedPiece, 0, 1); // Up
    checkDirection(checkedPiece, 0, -1); // Down
    checkDirection(checkedPiece, 1, 1); // Up-Right
    checkDirection(checkedPiece, -1, 1); // Up-Left
    checkDirection(checkedPiece, 1, -1); // Down-Right
    checkDirection(checkedPiece, -1, -1); // Down-Left
}

void BoardLogic::checkKingMoves(Piece *checkedPiece) {
    validTiles.clear(); // Empty out valid tiles
    captureTiles.clear(); // Empty out capture tiles

    auto addMoveOrCapture = [&](int x, int y) {
        // Check if the tile is within the board bounds
        if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
            // Check if the tile is occupied by an enemy piece
            auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece *piece) {
                return piece->gridPosition == glm::ivec2(x, y);
            });

            if (it == boardState.end()) {
                // Tile is empty
                validTiles.emplace_back(x, y);
            } else if ((*it)->pieceColor != checkedPiece->pieceColor) {
                // Tile is occupied by an enemy piece
                captureTiles.emplace_back(x, y);
            }
        }
    };

    // List all possible moves for a king
    std::vector<glm::ivec2> kingMoves = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Horizontal and vertical moves
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonal moves
    };

    for (const auto &move: kingMoves) {
        addMoveOrCapture(checkedPiece->gridPosition.x + move.x, checkedPiece->gridPosition.y + move.y);
    }
};

Piece *BoardLogic::findKing(PieceColor pieceColor) {
    for (Piece *piece: boardState) {
        if (piece->pieceType == KING && piece->pieceColor == pieceColor) {
            return piece;
        }
    }
    return nullptr;
}


bool BoardLogic::isKingInCheck(Piece *king) {
    for (Piece *piece: boardState) {
        if (piece->pieceColor != king->pieceColor) {

            switch (piece->pieceType) {
                case PAWN:
                    getPawnCaptureMoves(piece);
                    break;
                case ROOK:
                    getRookCaptureMoves(piece);
                    break;
                case QUEEN:
                    getQueenCaptureMoves(piece);
                    break;
                case BISHOP:
                    getBishopCaptureMoves(piece);
                    break;
                case KNIGHT:
                    getKnightCaptureMoves(piece);
                    break;
                case KING:
                    getKingCaptureMoves(piece);
                    break;
            }
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
    // Remove elements from validTiles based on condition
    validTiles.erase(
        std::remove_if(validTiles.begin(), validTiles.end(),
            [&](const glm::ivec2& tile) {
                return isKingInCheckAfterMove(piece, tile);
            }),
        validTiles.end()
    );

    // Remove elements from validTiles based on condition
    captureTiles.erase(
        std::remove_if(captureTiles.begin(), captureTiles.end(),
            [&](const glm::ivec2& tile) {
                return isKingInCheckAfterMove(piece, tile);
            }),
        captureTiles.end()
    );
}

bool BoardLogic::isKingInCheckAfterMove(Piece *piece, const glm::ivec2 &newPos) {
    // Save the original position
    glm::ivec2 originalPos = piece->gridPosition;

    // Temporarily move the piece
    piece->gridPosition = newPos;

    // Temporarily remove the captured piece if present
    Piece* capturedPiece = nullptr;
    auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece* p) {
        return p->gridPosition == newPos && p->pieceColor != piece->pieceColor;
    });
    if (it != boardState.end()) {
        capturedPiece = *it;
        boardState.erase(it);
    }

    // Check if the king is in check after the move
    Piece* king = findKing(piece->pieceColor);
    bool inCheck = isKingInCheck(king);

    // Revert the piece to its original position
    piece->gridPosition = originalPos;

    // Restore the captured piece if it was removed
    if (capturedPiece) {
        boardState.push_back(capturedPiece);
    }

    return inCheck;
}

void BoardLogic::getPawnCaptureMoves(Piece* checkedPiece) {
    potentialCaptureTiles.clear(); // Empty out potential capture tiles

    int direction = (checkedPiece->pieceColor == BLACK) ? 1 : -1;
    auto addCaptureIfValid = [&](int x, int y) {
        if (std::any_of(boardState.begin(), boardState.end(), [&](Piece* piece) {
            return piece->gridPosition == glm::ivec2(x, y) && piece->pieceColor != checkedPiece->pieceColor;
        })) {
            potentialCaptureTiles.emplace_back(x, y);
        }
    };

    addCaptureIfValid(checkedPiece->gridPosition.x - 1, checkedPiece->gridPosition.y + direction);
    addCaptureIfValid(checkedPiece->gridPosition.x + 1, checkedPiece->gridPosition.y + direction);
}

void BoardLogic::getRookCaptureMoves(Piece* checkedPiece) {
    potentialCaptureTiles.clear();

    auto checkDirection = [&](int dx, int dy) {
        int x = checkedPiece->gridPosition.x + dx;
        int y = checkedPiece->gridPosition.y + dy;
        while (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
            auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece* piece) {
                return piece->gridPosition == glm::ivec2(x, y);
            });

            if (it != boardState.end()) {
                if ((*it)->pieceColor != checkedPiece->pieceColor) {
                    potentialCaptureTiles.emplace_back(x, y);
                }
                break; // Stop further movement in this direction
            }

            x += dx;
            y += dy;
        }
    };

    checkDirection(1, 0); // Right
    checkDirection(-1, 0); // Left
    checkDirection(0, 1); // Up
    checkDirection(0, -1); // Down
}

void BoardLogic::getQueenCaptureMoves(Piece* checkedPiece) {
    potentialCaptureTiles.clear();

    auto checkDirection = [&](int dx, int dy) {
        int x = checkedPiece->gridPosition.x + dx;
        int y = checkedPiece->gridPosition.y + dy;
        while (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
            auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece* piece) {
                return piece->gridPosition == glm::ivec2(x, y);
            });

            if (it != boardState.end()) {
                if ((*it)->pieceColor != checkedPiece->pieceColor) {
                    potentialCaptureTiles.emplace_back(x, y);
                }
                break; // Stop further movement in this direction
            }

            x += dx;
            y += dy;
        }
    };

    checkDirection(1, 0); // Right
    checkDirection(-1, 0); // Left
    checkDirection(0, 1); // Up
    checkDirection(0, -1); // Down
    checkDirection(1, 1); // Up-Right
    checkDirection(-1, 1); // Up-Left
    checkDirection(1, -1); // Down-Right
    checkDirection(-1, -1); // Down-Left
}

void BoardLogic::getBishopCaptureMoves(Piece* checkedPiece) {
    potentialCaptureTiles.clear();

    auto checkDirection = [&](int dx, int dy) {
        int x = checkedPiece->gridPosition.x + dx;
        int y = checkedPiece->gridPosition.y + dy;
        while (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
            auto it = std::find_if(boardState.begin(), boardState.end(), [&](Piece* piece) {
                return piece->gridPosition == glm::ivec2(x, y);
            });

            if (it != boardState.end()) {
                if ((*it)->pieceColor != checkedPiece->pieceColor) {
                    potentialCaptureTiles.emplace_back(x, y);
                }
                break; // Stop further movement in this direction
            }

            x += dx;
            y += dy;
        }
    };

    checkDirection(1, 1); // Up-Right
    checkDirection(-1, 1); // Up-Left
    checkDirection(1, -1); // Down-Right
    checkDirection(-1, -1); // Down-Left
}

void BoardLogic::getKnightCaptureMoves(Piece* checkedPiece) {
    potentialCaptureTiles.clear();

    auto addCaptureIfValid = [&](int x, int y) {
        if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
            if (std::any_of(boardState.begin(), boardState.end(), [&](Piece* piece) {
                return piece->gridPosition == glm::ivec2(x, y) && piece->pieceColor != checkedPiece->pieceColor;
            })) {
                potentialCaptureTiles.emplace_back(x, y);
            }
        }
    };

    std::vector<glm::ivec2> knightMoves = {
        { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 },
        { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 }
    };

    for (const auto& move : knightMoves) {
        addCaptureIfValid(checkedPiece->gridPosition.x + move.x, checkedPiece->gridPosition.y + move.y);
    }
}

void BoardLogic::getKingCaptureMoves(Piece* checkedPiece) {
    potentialCaptureTiles.clear();

    auto addCaptureIfValid = [&](int x, int y) {
        if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
            if (std::any_of(boardState.begin(), boardState.end(), [&](Piece* piece) {
                return piece->gridPosition == glm::ivec2(x, y) && piece->pieceColor != checkedPiece->pieceColor;
            })) {
                potentialCaptureTiles.emplace_back(x, y);
            }
        }
    };

    std::vector<glm::ivec2> kingMoves = {
        { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, // Horizontal and vertical moves
        { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } // Diagonal moves
    };

    for (const auto& move : kingMoves) {
        addCaptureIfValid(checkedPiece->gridPosition.x + move.x, checkedPiece->gridPosition.y + move.y);
    }
}

std::vector<Piece*> BoardLogic::getBotPieces(PieceColor color) {
    std::vector<Piece*> botPieces;
    for (Piece* piece : boardState) {
        if (piece->pieceColor == color) {
            botPieces.push_back(piece);
        }
    }
    return botPieces;
}

void BoardLogic::selectRandomMove(Piece* piece) {
    // Clear the valid and capture tiles
    validTiles.clear();
    captureTiles.clear();

    selectedPiece = piece;

    // Get valid moves for the piece
    switch (piece->pieceType) {
        case PAWN: checkPawnMoves(selectedPiece); break;
        case ROOK: checkRookMoves(selectedPiece); break;
        case QUEEN: checkQueenMoves(selectedPiece); break;
        case BISHOP: checkBishopMoves(selectedPiece); break;
        case KNIGHT: checkKnightMoves(selectedPiece); break;
        case KING: checkKingMoves(selectedPiece); break;
    }

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
    } else {
        makeBotMove();
    }
}

void BoardLogic::makeBotMove() {
    PieceColor botColor = BLACK; // Change to BLACK if the bot plays as black
    std::vector<Piece*> botPieces = getBotPieces(botColor);

    if (!botPieces.empty()) {
        // Select a random piece
        int randomIndex = std::rand() % botPieces.size();
        std::cout << randomIndex << std::endl;
        Piece* randomPiece = botPieces[randomIndex];
        selectRandomMove(randomPiece);
    }
}
