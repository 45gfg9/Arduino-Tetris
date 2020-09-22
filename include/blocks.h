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

class TBlock : public Block
{
};

class IBlock : public Block
{
};

class LBlock : public Block
{
};

class ZBlock : public Block
{
};

class FlipLBlock : public Block
{
};

class FlipZBlock : public Block
{
};

class SqBlock : public Block
{
};
#endif
