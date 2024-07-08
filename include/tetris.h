/* Copyright © 2024 45gfg9 <45gfg9@45gfg9.net>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#ifndef __TETRIS__
#define __TETRIS__

#include <U8g2lib.h>

// pins used by display
#define PIN_CS U8X8_PIN_NONE
#define PIN_DC 9
#define PIN_RST 10

// ms per tile drop, the lower the faster
#define GAME_SPEED 1200

// check for input every 75ms
#define TICK_MS 75

// tetris input
enum input_t : uint8_t {
  TR_IN_LEFT,    // left shift
  TR_IN_RIGHT,   // right shift
  TR_IN_DOWN,    // non-locking soft drop
  TR_IN_ROT_CW,  // clockwise rotate
  TR_IN_ROT_CCW, // counterclockwise rotate
  TR_IN_LOCK,    // locking hard drop
  TR_IN_HOLD,    // hold
  TR_IN_PAUSE,   // pause
  TR_IN_NONE,    // no input
};

enum gamemode_t : uint8_t { MODE_STARTED, MODE_AUTO, MODE_IDLE, MODE_OVER };

// display
extern U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2;

// current game mode
extern gamemode_t mode;

void startGame(bool isAuto);
void enterIdleMode();

void tickGame();

input_t getInput();
void handleInput(input_t input);

#endif
