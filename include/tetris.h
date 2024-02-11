#ifndef __TETRIS__
#define __TETRIS__

#include <U8g2lib.h>

#define SCR_WIDTH 64
#define SCR_HEIGHT 128

#define PIN_CS U8X8_PIN_NONE
#define PIN_DC 9
#define PIN_RST 10

// tetris input
enum input_t : uint8_t {
  TR_IN_NONE,
  TR_IN_LEFT,
  TR_IN_RIGHT,
  TR_IN_DOWN,
  TR_IN_UP,
  TR_IN_START,
  TR_IN_SELECT,
};

void drawGameFrame(u8g2_t *u8g2);

// draw a 6x6 tile
void drawTile(u8g2_t *u8g2, uint8_t x, uint8_t y, uint8_t tile_idx);

[[noreturn]] void testLoop(u8g2_t *u8g2);

input_t getInput();

#endif
