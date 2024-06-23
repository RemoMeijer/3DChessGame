#ifndef BOARD_H
#define BOARD_H

#include "Drawable.h"
#include "Texture.h"

class Texture;

class Board : public Drawable {
private:
	Texture *texture;

public:
	Board(Texture *);

	~Board();

	virtual void init() override;

	virtual void draw() override;
};

#endif
