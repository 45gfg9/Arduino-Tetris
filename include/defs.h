#ifndef __TETRIS_DEFS__
#define __TETRIS_DEFS__

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 *gameboard;
extern Adafruit_SSD1306 *scoreboard;

struct Pos {
  byte x;
  byte y;

  bool operator==(const Pos &o) { return x == o.x && y == o.y; }
  bool operator!=(const Pos &o) { return x != o.x || y != o.y; }
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
