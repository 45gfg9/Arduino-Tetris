#ifndef __TETRIS_DEFS__
#define __TETRIS_DEFS__

#include <Adafruit_GFX.h>
#include "util.h"

typedef byte Pos;
typedef byte ID;
typedef Adafruit_GFX Painter;

enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

#endif
