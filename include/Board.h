#ifndef __TETRIS_BOARD__
#define __TETRIS_BOARD__

#include "Paintable.h"

class Board : public Paintable
{
    Pos x, y;

public:
    Board(Painter *painter, Pos x, Pos y);

    void paint() const;
};

#endif
