#include <SPI.h>
#include "tetris.h"

// D0 - 13 (SCK)
// D1 - 11 (MOSI)
Adafruit_SSD1306 *gameboard =
    new Adafruit_SSD1306(SCR_WIDTH, SCR_HEIGHT, &SPI, DC, RST, CS);
// Adafruit_SSD1306 *scoreboard = new Adafruit_SSD1306(SCR_WIDTH, 32);

void setup() {
  if (!(gameboard->begin()))
    for (;;)
      ;

  gameboard->clearDisplay();
  gameboard->drawRect(0, 0, 129, 64, SSD1306_WHITE);

  gameboard->display();
}

void loop() {}
