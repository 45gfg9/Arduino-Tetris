#ifndef __TETRIS_BOARD__
#define __TETRIS_BOARD__

#include <BitSet.h>
#include "Paintable.h"

class Board : public Paintable {
  BitSet *const _layout;

public:
  static const Pos SIZE;

  Board();
  ~Board();

  void set(const Pos &pos);
  bool get(const Pos &pos) const;

  bool isOutOfBounds(const Pos &pos) const;

  void paint() const;
};

extern Board board;

#endif
