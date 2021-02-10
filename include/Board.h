#ifndef __TETRIS_BOARD__
#define __TETRIS_BOARD__

#include "Paintable.h"

static const byte BOARD_X = 8;
static const byte BOARD_Y = 16;

class Board : public Paintable {
  bool _layout[BOARD_X][BOARD_Y];

public:
  const Pos size;
  Board(const Pos &size);
  ~Board();

  bool isEmpty(Pos pos) const;
  bool isOutOfBounds(Pos pos) const;

  void paint() const;
};

#endif
