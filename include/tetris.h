#ifndef __TETRIS__
#define __TETRIS__

#include <U8g2lib.h>

#define SCR_WIDTH 64
#define SCR_HEIGHT 128

#define PIN_CS U8X8_PIN_NONE
#define PIN_DC 9
#define PIN_RST 10

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

void drawGameFrame();

[[noreturn]] void testLoop();

input_t getInput();
void handleInput(input_t input);

#endif
