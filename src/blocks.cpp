#include "blocks.h"

static Pos calcRotLandPos(const Pos &pvt, const Pos &now, Rotation r) {
  if (r == Rotation::CLOCKWISE) {
    return {(byte)(now.y - pvt.y + pvt.x), (byte)(now.x - pvt.x + pvt.y)};
  } else {
    return {(byte)(pvt.y - now.y + pvt.x), (byte)(pvt.x - now.x + pvt.y)};
  }
}

Block::Block() {
  for (int i = 0; i < 4; i++)
    tiles[i] = nullptr;
}

Block::~Block() {
  for (int i = 0; i < 4; i++) {
    delete tiles[i];
  }
}

inline Tile *Block::getPivot() const {
  return tiles[0];
}

void Block::rotate(Rotation r) {
  Tile *pivot = getPivot();
  if (!pivot)
    return;

  for (int i = 1; i < 4; i++) {
    Tile *tile = tiles[i];

    // BS
    tile->_pos = calcRotLandPos(pivot->_pos, tile->_pos, r);
    tile->paint();
  }
}

bool Block::move(Direction d) {
  Tile *newTiles[4];
  for (int i = 0; i < 4; i++) {
    Tile *tile = tiles[i];

    // Not implemented
  }

  return true;
}

TBlock::TBlock() : Block() {}

IBlock::IBlock() : Block() {}

LBlock::LBlock() : Block() {}

ZBlock::ZBlock() : Block() {}

JBlock::JBlock() : Block() {}

FlipZBlock::FlipZBlock() : Block() {}

OBlock::OBlock() : Block() {}
