// travmonkey

#include "keymap.hpp"
#include "class/hid/hid.h"
#include <cstddef>
#include <cstdint>

KeyBoard::KeyBoard(const std::string &name) {
  // Initialize all of the keys depending on which half is stated
  if (name == "left") {
    // Initialize the left half of keys
    uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS] = {
      {
        {HID_KEY_TAB, '\0', '\0', '\0'},
        {HID_KEY_Q, HID_KEY_1, HID_KEY_F1, '\0'},
        {HID_KEY_W, HID_KEY_2, HID_KEY_F2, '\0'},
        {HID_KEY_E, HID_KEY_2, HID_KEY_F2, '\0'},
        {HID_KEY_R, HID_KEY_2, HID_KEY_F2, '\0'},
        {HID_KEY_T, HID_KEY_2, HID_KEY_F2, '\0'},
        {'\0', '\0', '\0', '\0'},
      },
      {
        {HID_KEY_SHIFT_LEFT, '\0', '\0', '\0'}
      },
      {
        {HID_KEY_SHIFT_LEFT, '\0', '\0', '\0'}
      },
      {
        {'\0', '\0', '\0', '\0'},
        {'\0', '\0', '\0', '\0'},
        {'\0', '\0', '\0', '\0'},
        {HID_KEY_MENU, '\0', '\0', '\0'},
        {HID_KEY_SHIFT_LEFT, '\0', '\0', '\0'}
      },
    };
    createKeymaps(keys);
  } else if (name == "right") {
    // Initialize the right half of keys
    uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS] = {
      {
        {'\0', '\0', '\0', '\0'},
        {HID_KEY_Q, HID_KEY_1, HID_KEY_F1, '\0'}
      },
      {
        {HID_KEY_SHIFT_LEFT, '\0', '\0', '\0'}
      }
    };
    createKeymaps(keys);
  }
};

void KeyBoard::createKeymaps(uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS]) {
  for (int r = 0; r < NUM_ROWS; r++) {
    for (int c = 0; c < NUM_COLUMNS; c++) {
      for (int l = 0; l < NUM_LAYERS; l++) {
        keymaps[r][c][l] = keys[r][c][l];
      }
    }
  }
}

uint8_t KeyBoard::returnKeycode() { return keymaps[0][0][0]; }

void KeyBoard::refresh() {
  // First clear the key code cache from the previous send
  for (size_t i = 0; i < 6; i++) {
    KeyBoard::key_codes[i] = 0;
  }
}
