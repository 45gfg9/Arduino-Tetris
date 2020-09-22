#include "Tile.h"

const unsigned char Tile::TEXTURE[8] = {0xff, 0x99, 0xbd, 0xe7, 0xe7, 0xbd, 0x99, 0xff};

Tile::Tile(Painter *painter, Pos x, Pos y) : x(x), y(y)
{
    this->painter = painter;
}
