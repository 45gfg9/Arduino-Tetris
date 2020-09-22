#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>

#include "tetris.h"

#define SCR_WIDTH 128
#define SCR_HEIGHT 64
#define CS 8
#define DC 9
#define RST 10

// D0 - 13 (SCK)
// D1 - 11 (MOSI)
Adafruit_SSD1306 display(SCR_WIDTH, SCR_HEIGHT, &SPI, DC, RST, CS);

void setup()
{
    if (!display.begin())
        for (;;)
            ;

}

void loop()
{
}
