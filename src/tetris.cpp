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

// hold box
// the highest bit indicates whether it was just swapped with current tile
static uint8_t holdBoxTile;
static uint8_t currentTile;

unsigned int score;
unsigned long gameTicks;

// board array, 4 bits tile index per block
static uint8_t gameBoard[BOARD_W * BOARD_H / 2];

uint8_t getBoard(uint8_t x, uint8_t y) {
  uint8_t b = gameBoard[y * BOARD_W + x];
  return x % 2 ? b >> 4 : b & 0x0f;
}

void setBoard(uint8_t x, uint8_t y, uint8_t tile_idx) {
  uint8_t i = y * BOARD_W + x;
  gameBoard[i] = x % 2 ? ((gameBoard[i] & 0x0f) | (tile_idx << 4)) : ((gameBoard[i] & 0xf0) | tile_idx);
}

void drawTile(uint8_t x, uint8_t y, uint8_t tile_idx) {
  x = TILESIZ * x + 2;
  y = TILESIZ * y + 12;

  if (tile_idx == 0) {
    u8g2.setDrawColor(0);

    u8g2.drawBox(x, y, TILESIZ, TILESIZ);
  } else {
    uint16_t b = pgm_read_word(TILES + tile_idx - 1);

    u8g2.drawFrame(x, y, TILESIZ, TILESIZ);
    for (uint8_t i = TILESIZ - 2; i; --i) {
      for (uint8_t j = TILESIZ - 2; j; --j) {
        u8g2.setDrawColor(b & 1);
        u8g2.drawHVLine(x + j, y + i, 1, 0);
        b >>= 1;
      }
    }
  }
  u8g2.setDrawColor(1);
}

void testLoop() {
  while (1) {
    for (uint8_t i = 0; i < BOARD_H; i++) {
      for (uint8_t j = 0; j < BOARD_W; j++) {
        drawTile(j, i, rand() % 8);
      }
    }
    u8g2.sendBuffer();
    delay(2000);
  }
}

void drawGameFrame() {
  u8g2.clearBuffer();

  u8g2.setDrawColor(1);
  u8g2.drawFrame(0, 10, 64, 118);
}

void tickGame() {
  ++gameTicks;

  // TODO
}

void handleInput(input_t input) {
  // TODO
}
