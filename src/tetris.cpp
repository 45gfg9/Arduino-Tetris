/* Copyright © 2024 45gfg9 <45gfg9@45gfg9.net>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#include <stdint.h>
#include <avr/pgmspace.h>
#include "tetris.h"

#define BOARD_W 10
#define BOARD_H 19
#define TILESIZ 6

#define TICKS_PER_UNIT_TIME (GAME_SPEED / TICK_MS)

enum tile_t : int8_t {
  TILE_T,
  TILE_Z,
  TILE_S,
  TILE_J,
  TILE_L,
  TILE_I,
  TILE_O,
};

static const uint16_t TEXTURE_T = 0x0660;
static const uint16_t TEXTURE_Z = 0x5a5a;
static const uint16_t TEXTURE_S = 0x8421;
static const uint16_t TEXTURE_J = 0x6996;
static const uint16_t TEXTURE_L = 0x9669;
static const uint16_t TEXTURE_I = 0xffff;
static const uint16_t TEXTURE_O = 0x9009;
static const uint16_t TILES[] PROGMEM = {TEXTURE_T, TEXTURE_Z, TEXTURE_S, TEXTURE_J, TEXTURE_L, TEXTURE_I, TEXTURE_O};
static const char TILE_CHARS[] PROGMEM = {'T', 'Z', 'S', 'J', 'L', 'I', 'O'};

// [tile][currentRot][3 more blocks][x, y]
static const int8_t TILEPOS[7][4][3][2] PROGMEM = {
    // T
    {
        {{-1, 0}, {0, 1}, {1, 0}},
        {{0, -1}, {0, 1}, {1, 0}},
        {{-1, 0}, {0, -1}, {1, 0}},
        {{-1, 0}, {0, 1}, {0, -1}},
    },
    // Z
    {
        {{-1, 1}, {0, 1}, {1, 0}},
        {{0, -1}, {1, 1}, {1, 0}},
        {{-1, 0}, {1, -1}, {0, -1}},
        {{-1, 0}, {0, 1}, {-1, -1}},
    },
    // S
    {
        {{-1, 0}, {0, 1}, {1, 1}},
        {{1, -1}, {0, 1}, {1, 0}},
        {{-1, -1}, {0, -1}, {1, 0}},
        {{-1, 0}, {0, -1}, {-1, 1}},
    },
    // J
    {
        {{-1, 1}, {-1, 0}, {1, 0}},
        {{1, 1}, {0, 1}, {0, -1}},
        {{-1, 0}, {1, 0}, {1, -1}},
        {{0, 1}, {0, -1}, {-1, -1}},
    },
    // L
    {
        {{-1, 0}, {1, 1}, {1, 0}},
        {{0, -1}, {0, 1}, {1, -1}},
        {{-1, 0}, {-1, -1}, {1, 0}},
        {{0, -1}, {0, 1}, {-1, 1}},
    },
    // I
    {
        {{-1, 0}, {1, 0}, {2, 0}},
        {{0, 1}, {0, -1}, {0, -2}},
        {{-2, 0}, {-1, 0}, {1, 0}},
        {{0, 2}, {0, 1}, {0, -1}},
    },
    // O
    {
        {{0, -1}, {1, 0}, {1, -1}},
        {{0, -1}, {1, 0}, {1, -1}},
        {{0, -1}, {1, 0}, {1, -1}},
        {{0, -1}, {1, 0}, {1, -1}},
    },
};

// [isIBlock][isClockwise][currentRot][5 tests][x, y]
static const int8_t WALL_KICK_DATA[2][2][4][5][2] PROGMEM = {
    {
        {
            /* 0>>3 */ {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
            /* 1>>0 */ {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
            /* 2>>1 */ {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
            /* 3>>2 */ {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
        },
        {
            /* 0>>1 */ {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
            /* 1>>2 */ {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
            /* 2>>3 */ {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
            /* 3>>0 */ {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
        },
    },
    {
        {
            /* 0>>3 */ {{0, -1}, {-1, -1}, {2, -1}, {-1, 1}, {2, -2}},
            /* 1>>0 */ {{-1, 0}, {1, 0}, {-2, 0}, {1, 1}, {-2, -2}},
            /* 2>>1 */ {{0, 1}, {1, 1}, {-2, 1}, {1, -1}, {-2, 2}},
            /* 3>>2 */ {{1, 0}, {-1, 0}, {2, 0}, {-1, -1}, {2, 2}},
        },
        {
            /* 0>>1 */ {{1, 0}, {-1, 0}, {2, 0}, {-1, -1}, {2, 2}},
            /* 1>>2 */ {{0, -1}, {-1, -1}, {2, -1}, {-1, 1}, {2, -2}},
            /* 2>>3 */ {{-1, 0}, {1, 0}, {-2, 0}, {1, 1}, {-2, -2}},
            /* 3>>0 */ {{0, 1}, {1, 1}, {-2, 1}, {1, -1}, {-2, 2}},
        },
    },
};

// hold box
// the MSB indicates whether it was just swapped with current tile (therefore cannot be swapped again)
static uint8_t holdBoxTile;

// current tile, its position and rotation
static tile_t currentTile;
static uint8_t currentTileX;
static uint8_t currentTileY;
static uint8_t currentTileRot;

// is game paused
static bool isPaused;

gamemode_t mode;

// score
static unsigned int score;
// ticks
static uint8_t gameTicks;

// board array, 4 bits tile index per block
// -1 indicates empty
static int8_t board[BOARD_H][(BOARD_W + 1) / 2];

int8_t getBoard(uint8_t x, uint8_t y) {
  if (x < BOARD_W && y < BOARD_H) {
    int8_t b = board[y][x / 2];
    if (x % 2 == 0) {
      // do not `return b & 0x0f`, it breaks -1
      b <<= 4;
    }
    return b >> 4;
  }
  // assume the top is filled with walls
  // utilizes unsigned overflow
  return (x >= BOARD_W || y > SCHAR_MAX) ? 0 : -1;
}

static inline bool isPosEmpty(uint8_t x, uint8_t y) {
  return getBoard(x, y) == -1;
}

bool setBoard(uint8_t x, uint8_t y, int8_t tileIdx) {
  if (y < BOARD_H && isPosEmpty(x, y)) {
    int8_t *p = &board[y][x / 2];
    *p = (x % 2) ? ((tileIdx << 4) | (*p & 0x0f)) : ((*p & 0xf0) | (tileIdx & 0x0f));
    return true;
  }
  return false;
}

// draw a 6x6 tile
void drawTile(uint8_t x, uint8_t y, int8_t tileIdx) {
  if (x >= BOARD_W || y >= BOARD_H) {
    return;
  }

  x = TILESIZ * x + 2;
  y = TILESIZ * (BOARD_H - 1 - y) + 12;

  if (tileIdx == -1) {
    u8g2.setDrawColor(0);

    u8g2.drawBox(x, y, TILESIZ, TILESIZ);
  } else {
    uint16_t b = pgm_read_word(TILES + tileIdx);

    u8g2.setDrawColor(1);
    u8g2.drawFrame(x, y, TILESIZ, TILESIZ);
    for (uint8_t i = TILESIZ - 2; i; --i) {
      for (uint8_t j = TILESIZ - 2; j; --j) {
        u8g2.setDrawColor(b & 1);
        u8g2.drawHVLine(x + j, y + i, 1, 0);
        b >>= 1;
      }
    }
  }
}

static void drawCurrent(bool isWhite) {
  int8_t tileIdx = isWhite ? currentTile : -1;

  drawTile(currentTileX, currentTileY, tileIdx);
  for (uint8_t i = 0; i < 3; i++) {
    drawTile(currentTileX + (int8_t)pgm_read_byte(&TILEPOS[currentTile][currentTileRot][i][0]),
             currentTileY + (int8_t)pgm_read_byte(&TILEPOS[currentTile][currentTileRot][i][1]), tileIdx);
  }
}

static bool isNextPosOk(int8_t offX, int8_t offY, uint8_t newRot) {
  if (!isPosEmpty(currentTileX + offX, currentTileY + offY)) {
    return false;
  }
  for (uint8_t i = 0; i < 3; i++) {
    if (!isPosEmpty(currentTileX + offX + (int8_t)pgm_read_byte(&TILEPOS[currentTile][newRot][i][0]),
                    currentTileY + offY + (int8_t)pgm_read_byte(&TILEPOS[currentTile][newRot][i][1]))) {
      return false;
    }
  }

  return true;
}

static void rotateCurrent(bool isClockwise) {
  uint8_t newTileRot = (currentTileRot + (isClockwise ? 1 : 3)) % 4;

  const bool isI = currentTile == TILE_I;

  for (uint8_t i = 0; i < 5; i++) {
    int8_t tileOffX = (int8_t)pgm_read_byte(&WALL_KICK_DATA[isI][isClockwise][currentTileRot][i][0]);
    int8_t tileOffY = (int8_t)pgm_read_byte(&WALL_KICK_DATA[isI][isClockwise][currentTileRot][i][1]);

    if (isNextPosOk(tileOffX, tileOffY, newTileRot)) {
      currentTileRot = newTileRot;
      currentTileX += tileOffX;
      currentTileY += tileOffY;
      break;
    }
  }
}

static void drawScore() {
  char str[6];
  itoa(score, str, 10);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 40, 8);
  u8g2.setDrawColor(1);
  u8g2.drawStr(0, 8, str);
}

void drawGameFrame() {
  u8g2.clearBuffer();

  u8g2.setDrawColor(1);
  u8g2.drawFrame(0, 10, 64, 118);
}

void enterIdleMode() {
  mode = MODE_IDLE;
  drawGameFrame();

  score = 0;
  drawScore();

  u8g2.drawStr(15, 60, "Tetris");

  u8g2.sendBuffer();
}

static void nextTile(bool isHold) {
  uint8_t nextTile;
  if (isHold) {
    nextTile = currentTile | 0x80;
  } else {
    nextTile = random() % 7;
  }
  currentTile = (tile_t)(holdBoxTile & 0x7);
  holdBoxTile = nextTile;
  currentTileX = 4;
  currentTileY = BOARD_H;
  currentTileRot = 0;

  char str[] = {pgm_read_byte(TILE_CHARS + (holdBoxTile & 0x7)), 0};
  u8g2.setDrawColor(0);
  u8g2.drawBox(58, 0, 6, 8);
  u8g2.setDrawColor(1);
  u8g2.drawStr(58, 8, str);

  gameTicks = TICKS_PER_UNIT_TIME - 1;
}

void startGame(bool isAuto) {
  mode = isAuto ? MODE_AUTO : MODE_STARTED;
  drawGameFrame();
  drawScore();

  memset(board, -1, sizeof board);
  holdBoxTile = random() % 7;
  nextTile(false);

  isPaused = false;
}

static void checkElimination() {
  bool isLineEmpty = false;
  uint8_t eliminatedLines = 0;

  // find the lines that are either full or empty
  // if it's full, eliminate it and move the rest down
  // if it's empty, stop since the rest are empty too
  uint8_t y = 0;
  for (; y < BOARD_H && !isLineEmpty; y++) {
    isLineEmpty = true;
    bool isLineFull = true;
    for (uint8_t x = 0; x < BOARD_W && (isLineEmpty || isLineFull); x++) {
      if (isPosEmpty(x, y)) {
        isLineFull = false;
      } else {
        isLineEmpty = false;
      }
    }

    if (isLineFull) {
      memmove(board[y], board[y + 1], sizeof board - sizeof board[0] * (y + 1));
      memset(board[BOARD_H - 1], -1, sizeof board[0]);
      y--;
      eliminatedLines++;
    }
  }

  if (eliminatedLines) {
    // redraw the board, also empty the top lines
    for (uint8_t z = 0; z < y - 1 + eliminatedLines; z++) {
      for (uint8_t x = 0; x < BOARD_W; x++) {
        drawTile(x, z, getBoard(x, z));
      }
    }

    score += eliminatedLines * (eliminatedLines + 1) / 2;
    drawScore();
  }
}

void tickGame() {
  ++gameTicks;

  if (mode == MODE_AUTO) {
    auto rnd = random() & 0x3f;

    if (rnd < 7) {
      handleInput((input_t)rnd);
    }

    if (gameTicks % (TICKS_PER_UNIT_TIME / 2) == 0) {
      unsigned int ticks = gameTicks / (TICKS_PER_UNIT_TIME / 2);
      if (ticks % 2) {
        u8g2.setDrawColor(1);
        u8g2.drawStr(24, 8, "AUTO");
        u8g2.sendBuffer();
      } else {
        u8g2.setDrawColor(0);
        u8g2.drawBox(24, 0, 32, 8);
      }
    }
  }

  if (gameTicks % TICKS_PER_UNIT_TIME || isPaused) {
    return;
  }

  gameTicks = 0;

  if (isNextPosOk(0, -1, currentTileRot)) {
    drawCurrent(false);
    currentTileY--;
    drawCurrent(true);
    u8g2.sendBuffer();
  } else {
    bool ok = setBoard(currentTileX, currentTileY, currentTile);
    for (uint8_t i = 0; i < 3; i++) {
      ok &= setBoard(currentTileX + (int8_t)pgm_read_byte(&TILEPOS[currentTile][currentTileRot][i][0]),
                     currentTileY + (int8_t)pgm_read_byte(&TILEPOS[currentTile][currentTileRot][i][1]), currentTile);
    }
    if (ok) {
      checkElimination();
      nextTile(false);
    } else {
      u8g2.drawStr(24, 8, "OVER");
      mode = MODE_OVER;
      u8g2.sendBuffer();
    }
  }
}

void handleInput(input_t input) {
  if (input == TR_IN_PAUSE) {
    isPaused = !isPaused;
    if (isPaused) {
      u8g2.setDrawColor(1);
      u8g2.drawStr(24, 8, "PAUSED");
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(24, 0, 32, 8);
    }
  } else if (isPaused) {
    return;
  } else {
    drawCurrent(false);
    if (input == TR_IN_HOLD && ((holdBoxTile & 0x80) == 0)) {
      nextTile(true);
    } else if (input == TR_IN_LEFT && isNextPosOk(-1, 0, currentTileRot)) {
      currentTileX--;
    } else if (input == TR_IN_RIGHT && isNextPosOk(1, 0, currentTileRot)) {
      currentTileX++;
    } else if (input == TR_IN_DOWN && isNextPosOk(0, -1, currentTileRot)) {
      gameTicks = TICKS_PER_UNIT_TIME - 1;
    } else if (input == TR_IN_LOCK) {
      while (isNextPosOk(0, -1, currentTileRot)) {
        currentTileY--;
      }
      gameTicks = TICKS_PER_UNIT_TIME - 1;
    } else if (input == TR_IN_ROT_CW || input == TR_IN_ROT_CCW) {
      rotateCurrent(input == TR_IN_ROT_CW);
    }
    drawCurrent(true);
  }
  u8g2.sendBuffer();
}
