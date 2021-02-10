#ifndef __TETRIS_TILES__
#define __TETRIS_TILES__

#include "Paintable.h"

#define TILE_HEIGHT 6
#define TILE_WIDTH 6

class Tile : public Pos, public Paintable {
public:
  static const byte _TEXTURE[] PROGMEM;

  Tile(const Pos &pos);

  void paint() const;
};

#endif
