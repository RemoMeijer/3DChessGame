//
// Created by remo on 6/22/24.
//

#include "LegalSpacesBoard.h"

LegalSpacesBoard::LegalSpacesBoard(int rows, int cols, float tileSize)
    : rows(rows), cols(cols), tileSize(tileSize) {
    grid.resize(rows, std::vector<Tile>(cols, Tile(0, 0, tileSize)));
    init();
}

void LegalSpacesBoard::init() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float offsetX = j * tileSize;
            float offsetY = i * tileSize;
            grid[i][j] = Tile(offsetX + 1.48f, offsetY + 0.35f, tileSize);
        }
    }
}

void LegalSpacesBoard::draw() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].draw();
        }
    }
}

Tile& LegalSpacesBoard::getTile(int x, int y) {
    return grid[y][x]; // Access with y first for row-major order
}
