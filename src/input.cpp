#include "tetris.h"
#include <HardwareSerial.h>

input_t getInput() {
  // tolower
  int ch = Serial.read();
  if (ch == '\x1b' && Serial.available()) {
    // not just a plain ESC
    ch = Serial.read();

    // arrow sequences
    if (ch == '[') {
      ch = 0x80 | Serial.read();
    }
  }
  switch (ch) {
    // case 0x80 | 'A':
    case ' ':
    case '8':
      return TR_IN_LOCK;
    case 0x80 | 'B':
    case '2':
      return TR_IN_DOWN;
    case 0x80 | 'C':
    case '6':
      return TR_IN_RIGHT;
    case 0x80 | 'D':
    case '4':
      return TR_IN_LEFT;
    case '\x1b':
      return TR_IN_PAUSE;
    case 'z':
    case 'Z':
    case '3':
    case '7':
      return TR_IN_ROT_CCW;
    case 'x':
    case 'X':
    case '1':
    case '5':
    case '9':
      return TR_IN_ROT_CW;
    case 'c':
    case 'C':
    case '0':
      return TR_IN_HOLD;
  }

  return TR_IN_NONE;
}
