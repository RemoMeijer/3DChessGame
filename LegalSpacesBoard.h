//
// Created by remo on 6/22/24.
//

#ifndef LEGALSPACESBOARD_H
#define LEGALSPACESBOARD_H
#include "Drawable.h"
#include "Tile.h"

class LegalSpacesBoard : public Drawable {
    float offsetX = 7.28f;
    float offsetY = 5.0f;

public:
    LegalSpacesBoard(int rows, int cols, float tileSize);
    ~LegalSpacesBoard();

    virtual void init() override;
    virtual void draw() override;

    Tile& getTile(int x, int y);
private:
    int rows;
    int cols;
    float tileSize;
    std::vector<std::vector<Tile>> grid;
};



#endif //LEGALSPACESBOARD_H
