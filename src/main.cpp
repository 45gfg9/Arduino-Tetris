#include <SPI.h>
#include "tetris.h"

// D0 - 13 (SCK)
// D1 - 11 (MOSI)

U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R3, PIN_CS, PIN_DC, PIN_RST);

void setup() {
  Serial.begin(115200);

  srand(analogRead(A5) << 8 ^ analogRead(A0));

  u8g2.begin();

  initGameFrame(u8g2.getU8g2());
}

void loop() {
  testLoop(u8g2.getU8g2());
}
