#ifndef __TETRIS_DEFS__
#define __TETRIS_DEFS__

#include <Adafruit_SSD1306.h>

using ID = byte;
extern Adafruit_SSD1306 *gameboard;
extern Adafruit_SSD1306 *scoreboard;

struct Pos {
  byte x : 4;
  byte y : 4;
};

enum class Rotation {
  CLOCKWISE,
  COUNTERCLOCKWISE,
};

enum class Direction {
  LEFT,
  RIGHT,
  UP,
  DOWN,
};

#endif
