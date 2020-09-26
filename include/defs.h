#ifndef __TETRIS_DEFS__
#define __TETRIS_DEFS__

#include <Adafruit_GFX.h>
#include "util.h"

typedef byte ID;
typedef Adafruit_GFX Painter;

struct Pos
{
    byte x : 4;
    byte y : 4;
};

enum Rotation
{
    CLOCKWISE,
    COUNTERCLOCKWISE,
};

enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

#endif
