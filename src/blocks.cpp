#include "blocks.h"

static Pos calcRotLandPos(Pos p, Pos t, Rotation r)
{
    if (r == CLOCKWISE)
    {
        return {(byte)(t.y - p.y + p.x), (byte)(t.x - p.x + p.y)};
    }
    else
    {
        return {(byte)(p.y - t.y + p.x), (byte)(p.x - t.x + p.y)};
    }
}

Block::Block(Painter *painter)
{
    this->painter = painter;

    for (int i = 0; i < 4; i++)
        tiles[i] = nullptr;
}

Block::~Block()
{
    for (int i = 0; i < 4; i++)
    {
        delete tiles[i];
    }
}

inline Tile *Block::getPivot() const
{
    return tiles[0];
}

void Block::rotate(Rotation r)
{
    Tile *pivot = getPivot();
    if (!pivot)
        return;

    for (int i = 1; i < 4; i++)
    {
        Tile *tile = tiles[i];

        // BS
        Pos land = calcRotLandPos(pivot->pos, tile->pos, r);
        tile->pos = land;
        tile->paint();
    }
}

bool Block::move(Direction d)
{
    Tile *newTiles[4];
    for (int i = 0; i < 4; i++)
    {
        Tile *tile = tiles[i];

        // Not implemented
    }

    return true;
}

TBlock::TBlock(Painter *painter) : Block(painter)
{
}

IBlock::IBlock(Painter *painter) : Block(painter)
{
}

LBlock::LBlock(Painter *painter) : Block(painter)
{
}

ZBlock::ZBlock(Painter *painter) : Block(painter)
{
}

JBlock::JBlock(Painter *painter) : Block(painter)
{
}

FlipZBlock::FlipZBlock(Painter *painter) : Block(painter)
{
}

OBlock::OBlock(Painter *painter) : Block(painter)
{
}
