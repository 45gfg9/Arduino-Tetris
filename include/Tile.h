#ifndef __TETRIS_TILES__
#define __TETRIS_TILES__

#include "Paintable.h"

#define TILE_HEIGHT 8
#define TILE_WIDTH 8

class Tile : public Paintable
{
    static const byte _TEXTURE[8];

public:
    Pos pos;

    Tile(Painter *painter, Pos pos);
    ~Tile();

    void paint() const;
};

#endif
