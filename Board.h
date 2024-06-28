#ifndef BOARD_H
#define BOARD_H

#include "Drawable.h"
#include "Texture.h"

class Texture;

class Board : public Drawable {
	Texture *texture;

public:
	Board(Texture *);

	~Board();

	void init() override;
	void draw() override;
};

#endif
