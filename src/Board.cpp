#include "Board.h"

Board::Board(const Pos &pos) : size(pos) {
  memset(_layout, 0, BOARD_X * BOARD_Y);
}

Board::~Board() {}

bool Board::isOutOfBounds(Pos pos) const {
  return (pos.x < 0 || pos.y < 0 || pos.x >= BOARD_X || pos.y >= BOARD_Y);
}

bool Board::isEmpty(Pos pos) const {
  if (isOutOfBounds(pos))
    return false;
  return !(_layout[pos.x][pos.y]);
}
