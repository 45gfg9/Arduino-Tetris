#include "Tile.h"

const byte Tile::_TEXTURE[8] = {0xff, 0x99, 0xbd, 0xe7, 0xe7, 0xbd, 0x99, 0xff};

Tile::Tile(Painter *painter, Pos pos) : pos(pos)
{
    this->painter = painter;
}

Tile::~Tile()
{
    this->painter = nullptr;
}
