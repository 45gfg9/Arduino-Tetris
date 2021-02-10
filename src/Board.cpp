#include "Board.h"
#include "Tile.h"

const Pos Board::SIZE {10, 21};
Board board;

Board::Board() : _layout(new BitSet(SIZE.x * SIZE.y)) {}

Board::~Board() {
  delete _layout;
}

bool Board::isOutOfBounds(const Pos &pos) const {
  return (pos.x < 0 || pos.y < 0 || pos.x >= SIZE.x || pos.y >= SIZE.y);
}

bool Board::get(const Pos &pos) const {
  return !isOutOfBounds(pos) && (_layout->get(pos.x + SIZE.x * pos.y));
}

void Board::paint() const {
  for (byte x = SIZE.x - 1; x; x--) {
    for (byte y = SIZE.y - 1; y; y--) {
      if (get({x, y})) {
        Tile({x, y}).paint();
      }
    }
  }
}
