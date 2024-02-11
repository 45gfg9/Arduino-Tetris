#include <stdint.h>
#include <avr/pgmspace.h>
#include "tetris.h"

#define BOARD_W 10
#define BOARD_H 19
#define TILESIZ 6

static const uint16_t TILE_O = 0x9009;
static const uint16_t TILE_T = 0x0660;
static const uint16_t TILE_Z = 0x5a5a;
static const uint16_t TILE_S = 0x8421;
static const uint16_t TILE_J = 0x6996;
static const uint16_t TILE_L = 0x9669;
static const uint16_t TILE_I = 0xffff;

static const uint16_t TILES[] PROGMEM = {TILE_O, TILE_T, TILE_Z, TILE_S, TILE_J, TILE_L, TILE_I};

// ticks
static unsigned long gameTicks;

void drawTile(u8g2_t *u8g2, uint8_t x, uint8_t y, uint8_t tile_idx) {
  x = TILESIZ * x + 2;
  y = TILESIZ * y + 12;

  if (tile_idx == 0) {
    u8g2->draw_color = 0;

    u8g2_DrawBox(u8g2, x, y, TILESIZ, TILESIZ);
  } else {
    uint16_t b = pgm_read_word(TILES + tile_idx - 1);

    u8g2_DrawFrame(u8g2, x, y, TILESIZ, TILESIZ);
    for (uint8_t i = TILESIZ - 2; i; --i) {
      for (uint8_t j = TILESIZ - 2; j; --j) {
        u8g2->draw_color = b & 1;
        u8g2_DrawHVLine(u8g2, x + j, y + i, 1, 0);
        b >>= 1;
      }
    }
  }
  u8g2->draw_color = 1;
}

void testLoop(u8g2_t *u8g2) {
  while (1) {
    for (uint8_t i = 0; i < BOARD_H; i++) {
      for (uint8_t j = 0; j < BOARD_W; j++) {
        drawTile(u8g2, j, i, rand() % 8);
      }
    }
    u8g2_SendBuffer(u8g2);
    delay(2000);
  }
}

void drawGameFrame(u8g2_t *u8g2) {
  u8g2_ClearBuffer(u8g2);

  u8g2->draw_color = 1;
  u8g2_DrawFrame(u8g2, 0, 10, 64, 118);
}
