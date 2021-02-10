#include <SPI.h>

#include "tetris.h"

#include <algorithm>

// D0 - 13 (SCK)
// D1 - 11 (MOSI)
Adafruit_SSD1306 *gameboard =
    new Adafruit_SSD1306(SCR_WIDTH, SCR_HEIGHT, &SPI, DC, RST, CS);
Adafruit_SSD1306 *scoreboard = new Adafruit_SSD1306(SCR_WIDTH, 32);

void setup() {
  if (!(gameboard->begin() && scoreboard->begin()))
    for (;;)
      ;

  // gameboard->clearDisplay();
  // gameboard->setTextColor(SSD1306_WHITE);
  // gameboard->drawLine(0, 0, 127, 63, SSD1306_WHITE);
  // gameboard->print("Text");
  gameboard->clearDisplay();
  gameboard->drawBitmap(0, 0, Tile::_TEXTURE, 6, 6, SSD1306_WHITE);
  gameboard->display();
  scoreboard->display();
}

void loop() {}
