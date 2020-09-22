#ifndef __TETRIS_TILES__
#define __TETRIS_TILES__

#include "Paintable.h"

#define TILE_HEIGHT 8
#define TILE_WIDTH 8

class Tile : public Paintable
{
    static const unsigned char TEXTURE[8];
    Tile *l, *r, *u, *d;
    Pos x, y;

public:
    Tile(Painter *painter, Pos x, Pos y);
    ~Tile();

    void paint() const;
};

#endif
