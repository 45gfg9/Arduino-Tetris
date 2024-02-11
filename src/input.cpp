#include "tetris.h"
#include <HardwareSerial.h>

input_t getInput() {
  // tolower
  switch (Serial.read() & 0xdf) {
    case 'W':
      return TR_IN_UP;
    case 'A':
      return TR_IN_LEFT;
    case 'S':
      return TR_IN_DOWN;
    case 'D':
      return TR_IN_RIGHT;
    case 'Z':
      return TR_IN_START;
    case 'X':
      return TR_IN_SELECT;
  }

  return TR_IN_NONE;
}
