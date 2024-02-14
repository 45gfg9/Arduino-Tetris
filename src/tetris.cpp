#include <stdint.h>
#include <avr/pgmspace.h>
#include "tetris.h"

#define BOARD_W 10
#define BOARD_H 19
#define TILESIZ 6

#define TICKS_PER_UNIT_TIME (1200 / TICK_MS)

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

// hold box
// the highest bit indicates whether it was just swapped with current tile
static uint8_t holdBoxTile;
static tile_t currentTile;

// current tile position and rotation
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
static int8_t gameBoard[BOARD_W * BOARD_H / 2];

int8_t getBoard(uint8_t x, uint8_t y) {
  if (x < BOARD_W && y < BOARD_H) {
    int8_t b = gameBoard[(y * BOARD_W + x) / 2];
    return (x % 2) ? b >> 4 : ((int8_t)(b << 4) >> 4);
  }
  return (x >= BOARD_W || y > 127) ? 0 : -1;
}

void setBoard(uint8_t x, uint8_t y, int8_t tileIdx) {
  if (x < BOARD_W || y < BOARD_H) {
    uint8_t i = (y * BOARD_W + x) / 2;
    gameBoard[i] = (x % 2) ? ((tileIdx << 4) | (gameBoard[i] & 0x0f)) : ((gameBoard[i] & 0xf0) | (tileIdx & 0x0f));
  }
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

static void drawCurrent(bool isWhite) {
  int8_t tileIdx = isWhite ? currentTile : -1;

  drawTile(currentTileX, currentTileY, tileIdx);
  for (uint8_t i = 0; i < 3; i++) {
    drawTile(currentTileX + (int8_t)pgm_read_byte(&TILEPOS[currentTile][currentTileRot][i][0]),
             currentTileY + (int8_t)pgm_read_byte(&TILEPOS[currentTile][currentTileRot][i][1]), tileIdx);
  }
}

static inline bool testPosRot(uint8_t x, uint8_t y) {
  return getBoard(x, y) == -1;
}

static bool isNextPositionOk(int8_t offX, int8_t offY, uint8_t newRot) {
  if (!testPosRot(currentTileX + offX, currentTileY + offY)) {
    return false;
  }
  for (uint8_t i = 0; i < 3; i++) {
    if (!testPosRot(currentTileX + offX + (int8_t)pgm_read_byte(&TILEPOS[currentTile][newRot][i][0]),
                    currentTileY + offY + (int8_t)pgm_read_byte(&TILEPOS[currentTile][newRot][i][1]))) {
      return false;
    }
  }

  return true;
}

// TODO: wall kicks
static void rotateCurrent(bool isClockwise) {
  uint8_t newTileRot = (currentTileRot + (isClockwise ? 1 : 3)) % 4;
  uint8_t tileOffX = 0;
  uint8_t tileOffY = 0;

  if (currentTile == TILE_I) {
    if ((currentTileRot == 0 && newTileRot == 1) || (currentTileRot == 3 && newTileRot == 2)) {
      tileOffX++;
    } else if ((currentTileRot == 1 && newTileRot == 0) || (currentTileRot == 2 && newTileRot == 3)) {
      tileOffX--;
    } else if ((currentTileRot == 0 && newTileRot == 3) || (currentTileRot == 1 && newTileRot == 2)) {
      tileOffY++;
    } else if ((currentTileRot == 3 && newTileRot == 0) || (currentTileRot == 2 && newTileRot == 1)) {
      tileOffY--;
    }
  }

  if (isNextPositionOk(tileOffX, tileOffY, newTileRot)) {
    currentTileRot = newTileRot;
    currentTileX += tileOffX;
    currentTileY -= tileOffY;
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

// return false on game over
static bool nextTile(bool isHold) {
  uint8_t nextTile;
  if (isHold) {
    nextTile = currentTile | 0x80;
  } else {
    nextTile = rand() % 7;
  }
  currentTile = (tile_t)(holdBoxTile & 0x7);
  holdBoxTile = nextTile;
  currentTileX = 4;
  currentTileY = BOARD_H - 1;
  currentTileRot = 0;

  if (!isNextPositionOk(0, 0, currentTileRot)) {
    return false;
  }

  char str[] = {pgm_read_byte(TILE_CHARS + (holdBoxTile & 0x7)), 0};
  u8g2.setDrawColor(0);
  u8g2.drawBox(58, 0, 6, 8);
  u8g2.setDrawColor(1);
  u8g2.drawStr(58, 8, str);
  return true;
}

void startGame(bool isAuto) {
  mode = isAuto ? MODE_AUTO : MODE_STARTED;
  drawGameFrame();
  drawScore();
  u8g2.sendBuffer();
  gameTicks = 0;

  memset(gameBoard, -1, sizeof gameBoard);
  holdBoxTile = rand() % 7;
  nextTile(false);
  drawCurrent(true);
  u8g2.sendBuffer();

  isPaused = false;
}

static void checkElimination() {
  const int BYTES_PER_ROW = BOARD_W / 2;
  bool isLineEmpty = false;
  uint8_t eliminatedLines = 0;

  uint8_t y = 0;
  for (; y < BOARD_H && !isLineEmpty; y++) {
    isLineEmpty = true;
    bool isLineFull = true;
    for (uint8_t x = 0; x < BOARD_W && (isLineEmpty || isLineFull); x++) {
      if (getBoard(x, y) == -1) {
        isLineFull = false;
      } else {
        isLineEmpty = false;
      }
    }

    if (isLineFull) {
      memmove(gameBoard + BYTES_PER_ROW * y, gameBoard + (BYTES_PER_ROW + 1) * y,
              sizeof gameBoard - (BYTES_PER_ROW + 1) * y);
      memset(gameBoard + sizeof gameBoard - BYTES_PER_ROW, -1, BYTES_PER_ROW);
      y--;
      eliminatedLines++;
    }
  }

  if (eliminatedLines) {
    // redraw the board
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
    auto rnd = rand() & 0x3f;

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

  if (isNextPositionOk(0, -1, currentTileRot)) {
    drawCurrent(false);
    currentTileY--;
    drawCurrent(true);
  } else {
    setBoard(currentTileX, currentTileY, currentTile);
    for (uint8_t i = 0; i < 3; i++) {
      setBoard(currentTileX + (int8_t)pgm_read_byte(&TILEPOS[currentTile][currentTileRot][i][0]),
               currentTileY + (int8_t)pgm_read_byte(&TILEPOS[currentTile][currentTileRot][i][1]), currentTile);
    }

    checkElimination();

    if (nextTile(false)) {
      drawCurrent(true);
    } else {
      Serial.println("game over");
    }
  }
  u8g2.sendBuffer();
}

void handleInput(input_t input) {
  if (input == TR_IN_PAUSE) {
    isPaused = !isPaused;
    if (isPaused) {
      u8g2.setDrawColor(1);
      u8g2.drawStr(24, 8, "PAUSED");
      u8g2.sendBuffer();
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(24, 0, 32, 8);
    }
  } else if (isPaused) {
    return;
  } else if (input == TR_IN_HOLD && ((holdBoxTile & 0x80) == 0)) {
    drawCurrent(false);
    nextTile(true);
    drawCurrent(true);
  } else {
    drawCurrent(false);
    if (input == TR_IN_LEFT && isNextPositionOk(-1, 0, currentTileRot)) {
      currentTileX--;
    } else if (input == TR_IN_RIGHT && isNextPositionOk(1, 0, currentTileRot)) {
      currentTileX++;
    } else if (input == TR_IN_DOWN && isNextPositionOk(0, -1, currentTileRot)) {
      currentTileY--;
      gameTicks = 0;
    } else if (input == TR_IN_LOCK) {
      while (isNextPositionOk(0, -1, currentTileRot)) {
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
