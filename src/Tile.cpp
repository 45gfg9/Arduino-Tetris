#include "Tile.h"

/*
******
**..**
*.**.*
*.**.*
**..**
******
*/

const byte Tile::_TEXTURE[] {
    0b11111100,
    0b11001100,
    0b10000100,
    0b10000100,
    0b11001100,
    0b11111100,
};

Tile::Tile(const Pos &pos) : _pos(pos) {}
