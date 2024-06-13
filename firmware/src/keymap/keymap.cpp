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
            {HID_KEY_E, HID_KEY_3, HID_KEY_F3, '\0'},
            {HID_KEY_R, HID_KEY_4, HID_KEY_F4, '\0'},
            {HID_KEY_T, HID_KEY_5, HID_KEY_F5, '\0'},
            {'\0', '\0', '\0', '\0'}, // Extra 1
        },
        {
            {HID_KEY_SHIFT_LEFT, '\0', '\0', '\0'},
            {HID_KEY_A, HID_KEY_1, HID_KEY_F1, '\0'},
            {HID_KEY_S, HID_KEY_2, HID_KEY_F2, '\0'},
            {HID_KEY_D, HID_KEY_3, HID_KEY_F3, '\0'},
            {HID_KEY_F, HID_KEY_4, HID_KEY_F4, '\0'},
            {HID_KEY_G, HID_KEY_5, HID_KEY_F5, '\0'},
            {'\0', '\0', '\0', '\0'}, // Extra 2
        },
        {
            {HID_KEY_CONTROL_LEFT, '\0', '\0', '\0'},
            {HID_KEY_Z, HID_KEY_1, HID_KEY_F1, '\0'},
            {HID_KEY_X, HID_KEY_2, HID_KEY_F2, '\0'},
            {HID_KEY_C, HID_KEY_3, HID_KEY_F3, '\0'},
            {HID_KEY_V, HID_KEY_4, HID_KEY_F4, '\0'},
            {HID_KEY_B, HID_KEY_5, HID_KEY_F5, '\0'},
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
        },
        {
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {HID_KEY_MENU, '\0', '\0', '\0'},
            {'\0', '\0', '\0', '\0'}, // Modifier key
            {HID_KEY_SPACE, '\0', '\0', '\0'},
        },
    };
    createKeymaps(keys);
  } else if (name == "right") {
    // Initialize the right half of keys
    uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS] = {
        {
            {'\0', '\0', '\0', '\0'}, // Extra 3
            {HID_KEY_Y, '\0', '\0', '\0'},
            {HID_KEY_U, HID_KEY_6, HID_KEY_F6, '\0'},
            {HID_KEY_I, HID_KEY_7, HID_KEY_F7, '\0'},
            {HID_KEY_O, HID_KEY_8, HID_KEY_F8, '\0'},
            {HID_KEY_P, HID_KEY_9, HID_KEY_F9, '\0'},
            {HID_KEY_BACKSPACE, HID_KEY_0, HID_KEY_F10, '\0'},
        },
        {
            {'\0', '\0', '\0', '\0'}, // Extra 4
            {HID_KEY_H, '\0', '\0', '\0'},
            {HID_KEY_J, HID_KEY_1, HID_KEY_F1, '\0'},
            {HID_KEY_K, HID_KEY_2, HID_KEY_F2, '\0'},
            {HID_KEY_L, HID_KEY_3, HID_KEY_F3, '\0'},
            {HID_KEY_SEMICOLON, HID_KEY_4, HID_KEY_F4, '\0'},
            {HID_KEY_ENTER, HID_KEY_5, HID_KEY_F5, '\0'},
        },
        {
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {HID_KEY_N, '\0', '\0', '\0'},
            {HID_KEY_M, HID_KEY_1, HID_KEY_F1, '\0'},
            {HID_KEY_COMMA, HID_KEY_2, HID_KEY_F2, '\0'},
            {HID_KEY_PERIOD, HID_KEY_3, HID_KEY_F3, '\0'},
            {HID_KEY_SLASH, HID_KEY_4, HID_KEY_F4, '\0'},
            {HID_KEY_SHIFT_RIGHT, HID_KEY_5, HID_KEY_F5, '\0'},
        },
        {
            {HID_KEY_MENU, '\0', '\0', '\0'},
            {'\0', '\0', '\0', '\0'}, // Modifier key
            {HID_KEY_SPACE, '\0', '\0', '\0'},
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
        },
    };
    // Call function to put the keys into the keymap variable
    createKeymaps(keys);
  }
};

// Add all of the keys to the keymap variable
void KeyBoard::createKeymaps(uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS]) {
  for (int r = 0; r < NUM_ROWS; r++) {
    for (int c = 0; c < NUM_COLUMNS; c++) {
      for (int l = 0; l < NUM_LAYERS; l++) {
        keymap[r][c][l] = keys[r][c][l];
      }
    }
  }
}

uint8_t KeyBoard::returnKeycode() { return keymap[0][5][0]; }

void KeyBoard::refresh() {
  // First clear the key code cache from the previous send
  for (size_t i = 0; i < 6; i++) {
    KeyBoard::key_codes[i] = 0;
  }
}
