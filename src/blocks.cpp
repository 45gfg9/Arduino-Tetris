#include "blocks.h"

static Pos calcRotLandPos(const Pos &pvt, const Pos &now, Rotation r) {
  switch (r) {
    case Rotation::CLOCKWISE:
      return {(byte)(now.y - pvt.y + pvt.x), (byte)(now.x - pvt.x + pvt.y)};
    case Rotation::COUNTERCLOCKWISE:
    default:
      return {(byte)(pvt.y - now.y + pvt.x), (byte)(pvt.x - now.x + pvt.y)};
  }
}

static Pos calcMoveLandPos(const Pos &pos, Direction d) {
  switch (d) {
    case Direction::LEFT:
      return {(byte)(pos.x - 1), (byte)pos.y};
    case Direction::RIGHT:
      return {(byte)(pos.x + 1), (byte)pos.y};
    case Direction::UP:
      return {(byte)pos.x, (byte)(pos.y - 1)};
    case Direction::DOWN:
    default:
      return {(byte)pos.x, (byte)(pos.y + 1)};
  }
}

static void fillPosRelease(Tile *dest[4], Pos src[4]) {
  for (int i = 4; i; i--) {
    delete dest[i];
    dest[i] = new Tile(src[i]);
  }
}

static void release(Tile *tiles[4]) {
  for (int i = 3; i; i--) {
    delete tiles[i];
    tiles[i] = nullptr;
  }
}

Block::Block() {
  for (int i = 0; i < 4; i++)
    _tiles[i] = nullptr;
}

Block::~Block() {
  release(_tiles);
}

inline Tile *Block::getPivot() const {
  return _tiles[0];
}

bool Block::rotate(Rotation r) {
  Tile *pivot = getPivot();
  if (!pivot)
    return true;

  Pos newPos[4] = {*pivot};
  for (int i = 1; i < 4; i++) {
    Pos pos = newPos[i] = calcRotLandPos(*pivot, *_tiles[i], r);
    if (board.isOutOfBounds(pos) || board.get(pos)) {
      return false;
    }
  }
  fillPosRelease(_tiles, newPos);
  return true;
}

bool Block::move(Direction d) {
  Pos newPos[4];
  for (int i = 3; i; i--) {
    Pos pos = newPos[i] = calcMoveLandPos(*_tiles[i], d);
    if (board.isOutOfBounds(pos) || board.get(pos))
      return false;
  }

  fillPosRelease(_tiles, newPos);
  return true;
}

void Block::paint() const {
  for (int i = 4; i; i--) {
    _tiles[i]->paint();
  }
}

TBlock::TBlock() : Block() {}

IBlock::IBlock() : Block() {}

LBlock::LBlock() : Block() {}

ZBlock::ZBlock() : Block() {}

JBlock::JBlock() : Block() {}

FlipZBlock::FlipZBlock() : Block() {}

OBlock::OBlock() : Block() {
  Pos pos[4] = {{4, 0}, {5, 0}, {4, 1}, {5, 1}};
  fillPosRelease(_tiles, pos);
}
