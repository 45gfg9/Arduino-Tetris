#include <SPI.h>
#include "tetris.h"

// D0 - 13 (SCK)
// D1 - 11 (MOSI)

U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R3, PIN_CS, PIN_DC, PIN_RST);

// milliseconds
static unsigned long lastWake;
static unsigned long lastInput;

// current game mode
gamemode_t mode;

void enterAutoMode();
void enterIdleMode();
void startGame();

void tickGame();

void setup() {
  Serial.begin(115200);

  srand((analogRead(A5) << 8) ^ analogRead(A0));

  u8g2.begin();
  u8g2.setFont(u8g2_font_5x8_tr);

  enterIdleMode();
  lastInput = millis();
  lastWake = lastInput;
}

void loop() {
  input_t input = getInput();

  if (input == TR_IN_NONE) {
    if (millis() - lastInput >= 10000 && mode == MODE_IDLE) {
      enterAutoMode();
    }
  } else {
    lastInput = millis();
    if (mode == MODE_AUTO) {
      enterIdleMode();
    } else if (mode == MODE_STARTED) {
      handleInput(input);
    } else if (input == TR_IN_LOCK) {
      startGame();
    }
  }

  if (mode != MODE_IDLE) {
    tickGame();
  }

  lastWake += TICK_MS;
  delay(lastWake - millis());
}

void enterAutoMode() {
  mode = MODE_AUTO;
  drawGameFrame();
  u8g2.sendBuffer();
  gameTicks = 0;
}

void enterIdleMode() {
  mode = MODE_IDLE;
  drawGameFrame();

  score = 0;
  u8g2.drawStr(0, 8, "0");

  u8g2.drawStr(15, 60, "Tetris");
  u8g2.drawStr(12, 70, "@45gfg9");

  u8g2.sendBuffer();
}

void startGame() {
  mode = MODE_STARTED;
  gameTicks = 0;
}
