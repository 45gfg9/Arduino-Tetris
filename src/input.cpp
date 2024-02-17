/* Copyright © 2024 45gfg9 <45gfg9@45gfg9.net>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#include "tetris.h"
#include <HardwareSerial.h>

input_t getInput() {
  // tolower
  int c = Serial.read();
  if (c == '\x1b' && Serial.available()) {
    // not just a plain ESC
    c = Serial.read();

    // arrow sequences
    if (c == '[') {
      c = 0x80 | Serial.read();
    }
  }

  if (c == (0x80 | 'A') || c == ' ' || c == '8') {
    return TR_IN_LOCK;
  } else if (c == (0x80 | 'B') || c == '2') {
    return TR_IN_DOWN;
  } else if (c == (0x80 | 'C') || c == '6') {
    return TR_IN_RIGHT;
  } else if (c == (0x80 | 'D') || c == '4') {
    return TR_IN_LEFT;
  } else if (c == '\x1b') {
    return TR_IN_PAUSE;
  } else if (c == 'z' || c == 'Z' || c == '3' || c == '7') {
    return TR_IN_ROT_CCW;
  } else if (c == 'x' || c == 'X' || c == '1' || c == '5' || c == '9') {
    return TR_IN_ROT_CW;
  } else if (c == 'c' || c == 'C' || c == '0') {
    return TR_IN_HOLD;
  } else {
    return TR_IN_NONE;
  }
}
