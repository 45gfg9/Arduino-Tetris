#ifndef __TETRIS_BOARD__
#define __TETRIS_BOARD__

#include "Paintable.h"

static const byte BOARD_X = 6;
static const byte BOARD_Y = 8;

class Board : public Paintable
{
    bool _layout[BOARD_X][BOARD_Y];

public:
    const Pos size;
    Board(Painter *painter, Pos size);
    ~Board();

    bool isEmpty(Pos pos) const;
    bool isOutOfBounds(Pos pos) const;

    void paint() const;
};

#endif
