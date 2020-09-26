#include "Board.h"

Board::Board(Painter *painter, Pos pos) : size(pos)
{
    this->painter = painter;

    layout = calloc(pos.x * pos.y, 1);
}

Board::~Board()
{
    free(layout);
}
