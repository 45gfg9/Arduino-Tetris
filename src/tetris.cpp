#include <stdint.h>
#include <avr/pgmspace.h>
#include "tetris.h"

#define BOARD_W 10
#define BOARD_H 19
#define TILESIZ 6

enum tile_t : int8_t {
  TILE_O,
  TILE_T,
  TILE_Z,
  TILE_S,
  TILE_J,
  TILE_L,
  TILE_I,
};

static const uint16_t TEXTURE_O = 0x9009;
static const uint16_t TEXTURE_T = 0x0660;
static const uint16_t TEXTURE_Z = 0x5a5a;
static const uint16_t TEXTURE_S = 0x8421;
static const uint16_t TEXTURE_J = 0x6996;
static const uint16_t TEXTURE_L = 0x9669;
static const uint16_t TEXTURE_I = 0xffff;
static const uint16_t TILES[] PROGMEM = {TEXTURE_O, TEXTURE_T, TEXTURE_Z, TEXTURE_S, TEXTURE_J, TEXTURE_L, TEXTURE_I};
static const char TILE_CHARS[] PROGMEM = {'O', 'T', 'Z', 'S', 'J', 'L', 'I'};

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

unsigned int score;
unsigned long gameTicks;

// board array, 4 bits tile index per block
static uint8_t gameBoard[BOARD_W * BOARD_H / 2];

uint8_t getBoard(uint8_t x, uint8_t y) {
  uint8_t b = gameBoard[y * BOARD_W + x];
  return x % 2 ? b >> 4 : b & 0x0f;
}

void setBoard(uint8_t x, uint8_t y, uint8_t tileIdx) {
  uint8_t i = y * BOARD_W + x;
  gameBoard[i] = x % 2 ? ((gameBoard[i] & 0x0f) | (tileIdx << 4)) : ((gameBoard[i] & 0xf0) | tileIdx);
}

// draw a 6x6 tile
void drawTile(uint8_t x, uint8_t y, int8_t tileIdx) {
  if (x >= BOARD_W || y >= BOARD_H) {
    return;
  }

  x = TILESIZ * x + 2;
  y = TILESIZ * y + 12;

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

static void drawO(int8_t tileIdx) {
  for (uint8_t i = 0; i < 2; i++) {
    for (uint8_t j = 0; j < 2; j++) {
      drawTile(currentTileX + i, currentTileY + j, tileIdx);
    }
  }
}

static void drawT(int8_t tileIdx) {
  drawTile(currentTileX, currentTileY, tileIdx);
  if (currentTileRot != 0) {
    drawTile(currentTileX, currentTileY + 1, tileIdx);
  }
  if (currentTileRot != 1) {
    drawTile(currentTileX - 1, currentTileY, tileIdx);
  }
  if (currentTileRot != 2) {
    drawTile(currentTileX, currentTileY - 1, tileIdx);
  }
  if (currentTileRot != 3) {
    drawTile(currentTileX + 1, currentTileY, tileIdx);
  }
}

static void drawZ(int8_t tileIdx) {
  drawTile(currentTileX, currentTileY, tileIdx);
  switch (currentTileRot) {
    case 0:
      drawTile(currentTileX - 1, currentTileY - 1, tileIdx);
      drawTile(currentTileX, currentTileY - 1, tileIdx);
      drawTile(currentTileX + 1, currentTileY, tileIdx);
      break;
    case 1:
      drawTile(currentTileX + 1, currentTileY - 1, tileIdx);
      drawTile(currentTileX + 1, currentTileY, tileIdx);
      drawTile(currentTileX, currentTileY + 1, tileIdx);
      break;
    case 2:
      drawTile(currentTileX + 1, currentTileY + 1, tileIdx);
      drawTile(currentTileX, currentTileY + 1, tileIdx);
      drawTile(currentTileX - 1, currentTileY, tileIdx);
      break;
    case 3:
      drawTile(currentTileX - 1, currentTileY + 1, tileIdx);
      drawTile(currentTileX - 1, currentTileY, tileIdx);
      drawTile(currentTileX, currentTileY - 1, tileIdx);
      break;
  }
}

static void drawS(int8_t tileIdx) {
  drawTile(currentTileX, currentTileY, tileIdx);
  switch (currentTileRot) {
    case 0:
      drawTile(currentTileX + 1, currentTileY - 1, tileIdx);
      drawTile(currentTileX, currentTileY - 1, tileIdx);
      drawTile(currentTileX - 1, currentTileY, tileIdx);
      break;
    case 1:
      drawTile(currentTileX + 1, currentTileY + 1, tileIdx);
      drawTile(currentTileX + 1, currentTileY, tileIdx);
      drawTile(currentTileX, currentTileY - 1, tileIdx);
      break;
    case 2:
      drawTile(currentTileX - 1, currentTileY + 1, tileIdx);
      drawTile(currentTileX, currentTileY + 1, tileIdx);
      drawTile(currentTileX + 1, currentTileY, tileIdx);
      break;
    case 3:
      drawTile(currentTileX - 1, currentTileY - 1, tileIdx);
      drawTile(currentTileX - 1, currentTileY, tileIdx);
      drawTile(currentTileX, currentTileY + 1, tileIdx);
      break;
  }
}

static void drawJ(int8_t tileIdx) {
  drawTile(currentTileX, currentTileY, tileIdx);
  if (currentTileRot % 2) {
    drawTile(currentTileX, currentTileY - 1, tileIdx);
    drawTile(currentTileX, currentTileY + 1, tileIdx);

    if (currentTileRot == 1) {
      drawTile(currentTileX + 1, currentTileY - 1, tileIdx);
    } else {
      drawTile(currentTileX - 1, currentTileY + 1, tileIdx);
    }
  } else {
    drawTile(currentTileX - 1, currentTileY, tileIdx);
    drawTile(currentTileX + 1, currentTileY, tileIdx);

    if (currentTileRot == 0) {
      drawTile(currentTileX - 1, currentTileY - 1, tileIdx);
    } else {
      drawTile(currentTileX + 1, currentTileY + 1, tileIdx);
    }
  }
}

static void drawL(int8_t tileIdx) {
  drawTile(currentTileX, currentTileY, tileIdx);
  if (currentTileRot % 2) {
    drawTile(currentTileX, currentTileY - 1, tileIdx);
    drawTile(currentTileX, currentTileY + 1, tileIdx);

    if (currentTileRot == 1) {
      drawTile(currentTileX + 1, currentTileY + 1, tileIdx);
    } else {
      drawTile(currentTileX - 1, currentTileY - 1, tileIdx);
    }
  } else {
    drawTile(currentTileX - 1, currentTileY, tileIdx);
    drawTile(currentTileX + 1, currentTileY, tileIdx);

    if (currentTileRot == 0) {
      drawTile(currentTileX + 1, currentTileY - 1, tileIdx);
    } else {
      drawTile(currentTileX - 1, currentTileY + 1, tileIdx);
    }
  }
}

static void drawI(int8_t tileIdx) {
  drawTile(currentTileX, currentTileY, tileIdx);
  if (currentTileRot % 2) {
    drawTile(currentTileX, currentTileY - 1, tileIdx);
    drawTile(currentTileX, currentTileY + 1, tileIdx);

    if (currentTileRot == 1) {
      drawTile(currentTileX, currentTileY + 2, tileIdx);
    } else {
      drawTile(currentTileX, currentTileY - 2, tileIdx);
    }
  } else {
    drawTile(currentTileX - 1, currentTileY, tileIdx);
    drawTile(currentTileX + 1, currentTileY, tileIdx);

    if (currentTileRot == 0) {
      drawTile(currentTileX + 2, currentTileY, tileIdx);
    } else {
      drawTile(currentTileX - 2, currentTileY, tileIdx);
    }
  }
}

static void drawCurrent(bool isWhite) {
  static void (*const drawingFunctions[])(int8_t) PROGMEM = {drawO, drawT, drawZ, drawS, drawJ, drawL, drawI};

  int8_t tileIdx = isWhite ? currentTile : -1;

  void (*drawingFunction)(int8_t) = (void (*)(int8_t))pgm_read_ptr(drawingFunctions + currentTile);
  drawingFunction(tileIdx);
}

static void rotateCurrent(bool isClockwise) {
  uint8_t nextTileRot = (currentTileRot + (isClockwise ? 1 : 3)) % 4;

  if (currentTile == TILE_I) {
    if ((currentTileRot == 0 && nextTileRot == 1) || (currentTileRot == 3 && nextTileRot == 2)) {
      currentTileX++;
    } else if ((currentTileRot == 1 && nextTileRot == 0) || (currentTileRot == 2 && nextTileRot == 3)) {
      currentTileX--;
    } else if ((currentTileRot == 0 && nextTileRot == 3) || (currentTileRot == 1 && nextTileRot == 2)) {
      currentTileY++;
    } else if ((currentTileRot == 3 && nextTileRot == 0) || (currentTileRot == 2 && nextTileRot == 1)) {
      currentTileY--;
    }
  }

  currentTileRot = nextTileRot;
}

static void drawScore() {
  char str[6];
  itoa(score, str, 10);
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
    nextTile = rand() % 7;
  }
  currentTile = (tile_t)(holdBoxTile & 0x7);
  holdBoxTile = nextTile;
  currentTileX = 4;
  currentTileY = 0;
  currentTileRot = 0;

  char str[] = {pgm_read_byte(TILE_CHARS + (holdBoxTile & 0x7)), 0};
  u8g2.setDrawColor(0);
  u8g2.drawBox(58, 0, 6, 8);
  u8g2.setDrawColor(1);
  u8g2.drawStr(58, 8, str);
}

void startGame(bool isAuto) {
  mode = isAuto ? MODE_AUTO : MODE_STARTED;
  drawGameFrame();
  drawScore();
  u8g2.sendBuffer();
  gameTicks = 0;

  holdBoxTile = rand() % 7;
  nextTile(false);
  drawCurrent(true);
  u8g2.sendBuffer();

  memset(gameBoard, 0, sizeof gameBoard);
  isPaused = false;
}

void tickGame() {
  ++gameTicks;

  if (mode == MODE_AUTO && gameTicks % 10 == 0) {
    unsigned int ticks = gameTicks / 10;
    if (ticks % 2) {
      u8g2.setDrawColor(1);
      u8g2.drawStr(24, 8, "AUTO");
      u8g2.sendBuffer();
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(24, 0, 32, 8);
    }
  }

  if (gameTicks % 20 || isPaused) {
    return;
  }

  drawCurrent(false);
  currentTileY++;
  drawCurrent(true);
  u8g2.sendBuffer();
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
  } else if (input == TR_IN_HOLD && ((holdBoxTile & 0x80) == 0)) {
    drawCurrent(false);
    nextTile(true);
  } else {
    drawCurrent(false);
    if (input == TR_IN_LEFT) {
      currentTileX--;
    } else if (input == TR_IN_RIGHT) {
      currentTileX++;
    } else if (input == TR_IN_DOWN) {
      currentTileY++;
    } else if (input == TR_IN_LOCK) {
      // NYI
    } else if (input == TR_IN_ROT_CW || input == TR_IN_ROT_CCW) {
      rotateCurrent(input == TR_IN_ROT_CW);
    }
    drawCurrent(true);
  }
  u8g2.sendBuffer();
}
