#include <SPI.h>
#include "tetris.h"

#define TICK_MS 50

// D0 - 13 (SCK)
// D1 - 11 (MOSI)

enum gamemode_t : uint8_t { MODE_IDLE, MODE_AUTO, MODE_STARTED };

U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R3, PIN_CS, PIN_DC, PIN_RST);

// milliseconds
static unsigned long lastWake;
static unsigned long lastInput;

// score
static unsigned int score;

// current game mode
static gamemode_t mode;

void enterAutoMode();
void enterIdleMode();
void startGame();

void setup() {
  Serial.begin(115200);

  srand(analogRead(A5) << 8 ^ analogRead(A0));

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
    }
  }

  lastWake += TICK_MS;
  delay(lastWake - millis());
}

void enterAutoMode() {
  mode = MODE_AUTO;
  drawGameFrame(u8g2.getU8g2());
  u8g2.sendBuffer();
}

void enterIdleMode() {
  mode = MODE_IDLE;
  drawGameFrame(u8g2.getU8g2());

  score = 0;
  u8g2.drawStr(0, 8, "0");

  u8g2.drawStr(15, 60, "Tetris");
  u8g2.drawStr(12, 70, "@45gfg9");

  u8g2.sendBuffer();
}

void startGame() {
  mode = MODE_STARTED;
}
