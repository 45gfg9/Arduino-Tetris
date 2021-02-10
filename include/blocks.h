#ifndef __TETRIS_BLOCKS__
#define __TETRIS_BLOCKS__

#include "Paintable.h"
#include "Board.h"
#include "Tile.h"

class Block : public Paintable {
protected:
  Tile *_tiles[4];
  Block();

public:
  virtual ~Block();

  virtual Tile *getPivot() const;
  virtual bool rotate(Rotation d);
  virtual bool move(Direction d);

  void paint() const;
};

/*

    +---+
    | 2 |
+---+---+---+
| 1 | 0 | 3 |
+---+---+---+

*/
// T-Shaped Block
class TBlock : public Block {
public:
  TBlock();
};

/*

+---+---+---+---+
| 1 | 0 | 2 | 3 |
+---+---+---+---+

*/
// I-Shaped Block
class IBlock : public Block {
public:
  IBlock();
};

/*

        +---+
        | 3 |
+---+---+---+
| 1 | 0 | 2 |
+---+---+---+

*/
// L-Shaped Block
class LBlock : public Block {
public:
  LBlock();
};

/*

+---+---+
| 1 | 0 |
+---+---+---+
    | 2 | 3 |
    +---+---+

*/
// Z-Shaped Block
class ZBlock : public Block {
public:
  ZBlock();
};

/*

+---+
| 3 |
+---+---+---+
| 2 | 0 | 1 |
+---+---+---+

*/
// J-Shaped Block
class JBlock : public Block {
public:
  JBlock();
};

/*

    +---+---+
    | 0 | 1 |
+---+---+---+
| 2 | 3 |
+---+---+

*/
// Flipped Z-Shaped Block
class FlipZBlock : public Block {
public:
  FlipZBlock();
};

/*

+---+---+
| 0 | 1 |
+---+---+
| 2 | 3 |
+---+---+

*/
// O-Shaped(Square) Block
class OBlock : public Block {
public:
  OBlock();
  Tile *getPivot() const { return nullptr; }
  bool rotate(Rotation r) { return true; }
};

#endif
