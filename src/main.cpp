/* Copyright © 2024 45gfg9 <45gfg9@45gfg9.net>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#include <SPI.h>
#include "tetris.h"

// D0 - 13 (SCK)
// D1 - 11 (MOSI)

U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R3, PIN_CS, PIN_DC, PIN_RST);

// milliseconds
static unsigned long lastWake;
static unsigned long lastInput;

void setup() {
  Serial.begin(115200);

  unsigned long randomSeed = 0;
  for (uint8_t pin = A0; pin <= A7; pin++) {
    randomSeed = (randomSeed << 2) ^ analogRead(pin);
    delayMicroseconds(100);
  }
  srandom(randomSeed);

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
      startGame(true);
    }
  } else {
    lastInput = millis();
    if (mode == MODE_AUTO) {
      enterIdleMode();
    } else if (mode == MODE_STARTED) {
      handleInput(input);
    } else if (mode == MODE_OVER) {
      enterIdleMode();
    } else if (input == TR_IN_PAUSE) {
      startGame(false);
    }
  }

  if (mode != MODE_IDLE) {
    tickGame();
  }

  lastWake += TICK_MS;
  delay(lastWake - millis());
}
