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

static Pos boardPosToPixel(const Pos &pos) {
  return {(byte)(122 - 6 * pos.y), (byte)(2 + 6 * pos.x)};
}

Tile::Tile(const Pos &pos) : Pos(pos) {}

void Tile::paint() const {
  Pos pos = boardPosToPixel(*this);
  gameboard->drawBitmap(
      pos.x, pos.y, _TEXTURE, TILE_WIDTH, TILE_HEIGHT, SSD1306_WHITE);
}
