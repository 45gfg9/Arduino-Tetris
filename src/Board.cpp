#include "Board.h"

Board::Board(Painter *painter, Pos x, Pos y) : x(x), y(y)
{
    this->painter = painter;
}
