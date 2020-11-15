#include "blocks.h"

static Pos calcRotLandPos(Pos pvt, Pos now, Rotation r)
{
    if (r == CLOCKWISE)
    {
        return {(byte)(now.y - pvt.y + pvt.x), (byte)(now.x - pvt.x + pvt.y)};
    }
    else
    {
        return {(byte)(pvt.y - now.y + pvt.x), (byte)(pvt.x - now.x + pvt.y)};
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
        tile->pos = calcRotLandPos(pivot->pos, tile->pos, r);
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
