#ifndef __TETRIS__
#define __TETRIS__

#include <stdnoreturn.h>
#include <U8g2lib.h>

#define SCR_WIDTH 64
#define SCR_HEIGHT 128

#define PIN_CS U8X8_PIN_NONE
#define PIN_DC 9
#define PIN_RST 10

void initGameFrame(u8g2_t *u8g2);

// draw a 6x6 tile
void drawTile(u8g2_t *u8g2, uint8_t x, uint8_t y, uint8_t tile_idx);

[[noreturn]] void testLoop(u8g2_t *u8g2);

#endif
