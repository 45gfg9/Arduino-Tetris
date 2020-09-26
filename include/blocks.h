#ifndef __TETRIS_BLOCK__
#define __TETRIS_BLOCK__

#include "Paintable.h"
#include "Tile.h"

class Block : public Paintable
{
protected:
    Tile *t;

    struct IDedTile
    {
        ID id;
        Tile *tile;
    };

public:
    Block(Painter *painter);
    virtual ~Block() = 0;

    virtual Tile *getRotatePivot(Direction d) const = 0;

    int getBound(Direction d) const;
    virtual bool rotate(Direction d);
    virtual bool moveTo(Direction d);

    void paint() const;
};

/*

    +---+
    | 1 |
+---+---+---+
| 2 | 3 | 4 |
+---+---+---+

*/
// T-Shaped Block
class TBlock : public Block
{
};

/*

+---+---+---+---+
| 1 | 2 | 3 | 4 |
+---+---+---+---+

*/
// I-Shaped Block
class IBlock : public Block
{
};

/*

        +---+
        | 4 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+

*/
// L-Shaped Block
class LBlock : public Block
{
};

/*

+---+---+
| 1 | 2 |
+---+---+---+
    | 3 | 4 |
    +---+---+

*/
// Z-Shaped Block
class ZBlock : public Block
{
};

/*

+---+
| 4 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+

*/
// Flipped L-Shaped Block
class FlipLBlock : public Block
{
};

/*

    +---+---+
    | 1 | 2 |
+---+---+---+
| 3 | 4 |
+---+---+

*/
// Flipped Z-Shaped Block
class FlipZBlock : public Block
{
};

/*

+---+---+
| 1 | 2 |
+---+---+
| 3 | 4 |
+---+---+

*/
// Square Block
class SqBlock : public Block
{
};
#endif
