#ifndef __TETRIS_BOARD__
#define __TETRIS_BOARD__

#include "Paintable.h"

class Board : public Paintable
{
    void *layout;

public:
    Pos size;
    Board(Painter *painter, Pos size);
    ~Board();

    void paint() const;
};

#endif
