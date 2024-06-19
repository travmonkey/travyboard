// travmonkey

#include "keymap.hpp"
#include "class/hid/hid.h"
#include <cstdint>

KeyMap::KeyMap(const std::string &name) {
  // Initialize all of the keys depending on which half is stated
  if (name == "left") {
    // Initialize the left half of keys
    uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS] = {
        {
            // ROW 0
            {2, 2, 2, 2},
            {HID_KEY_Q, HID_KEY_1, HID_KEY_2, HID_KEY_3},
            {HID_KEY_W, HID_KEY_2, HID_KEY_F2, HID_KEY_F2},
            {HID_KEY_E, HID_KEY_3, HID_KEY_F3, HID_KEY_F3},
            {HID_KEY_R, HID_KEY_4, HID_KEY_F4, HID_KEY_F4},
            {HID_KEY_T, HID_KEY_5, HID_KEY_F5, HID_KEY_F5},
            {'\0', '\0', '\0', '\0'}, // Extra 1
        },
        {
            // ROW 1
            {HID_KEY_SHIFT_LEFT, HID_KEY_SHIFT_LEFT, HID_KEY_SHIFT_LEFT, HID_KEY_SHIFT_LEFT},
            {HID_KEY_A, '\0', '\0', '\0'},
            {1, 1, 1, 1},
            {HID_KEY_D, '\0', '\0', '\0'},
            {HID_KEY_F, '\0', '\0', '\0'},
            {HID_KEY_G, '\0', '\0', '\0'},
            {'\0', '\0', '\0', '\0'}, // Extra 2
        },
        {
             // ROW 2
            {HID_KEY_CONTROL_LEFT, '\0', '\0', '\0'},
            {HID_KEY_Z, '\0', '\0', '\0'},
            {HID_KEY_X, '\0', '\0', '\0'},
            {HID_KEY_C, '\0', '\0', '\0'},
            {HID_KEY_V, '\0', '\0', '\0'},
            {HID_KEY_B, '\0', '\0', '\0'},
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
        },
        {
            // ROW 3
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {HID_KEY_MENU, '\0', HID_KEY_ALT_LEFT, '\0'},
            {1, 1, 1, 1}, // Modifier key
            {HID_KEY_SPACE, HID_KEY_SPACE, HID_KEY_SPACE, HID_KEY_SPACE},
        },
    };
    create_keymaps(keys);
  } else if (name == "right") {
    // Initialize the right half of keys
    uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS] = {
        {
            {'\0', '\0', '\0', '\0'}, // Extra 3
            {HID_KEY_Y, HID_KEY_6, HID_KEY_F6, HID_KEY_F6},
            {HID_KEY_U, HID_KEY_7, HID_KEY_F7, HID_KEY_F7},
            {HID_KEY_I, HID_KEY_8, HID_KEY_BRACKET_LEFT, HID_KEY_F8},
            {HID_KEY_O, HID_KEY_9, HID_KEY_BRACKET_RIGHT, HID_KEY_F9},
            {HID_KEY_P, HID_KEY_0, HID_KEY_BACKSLASH, HID_KEY_F10},
            {HID_KEY_BACKSPACE, HID_KEY_MINUS, HID_KEY_EQUAL, '\0'},
        },
        {
            {'\0', '\0', '\0', '\0'}, // Extra 4
            {HID_KEY_H, HID_KEY_ARROW_LEFT, '\0', '\0'},
            {HID_KEY_J, HID_KEY_ARROW_DOWN, HID_KEY_VOLUME_DOWN, '\0'},
            {HID_KEY_K, HID_KEY_ARROW_UP, HID_KEY_VOLUME_UP, '\0'},
            {HID_KEY_L, HID_KEY_ARROW_RIGHT, '\0', '\0'},
            {HID_KEY_SEMICOLON, HID_KEY_APOSTROPHE, HID_KEY_F4, '\0'},
            {HID_KEY_ENTER, '\0', '\0', '\0'},
        },
        {
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {HID_KEY_N, '\0', '\0', '\0'},
            {HID_KEY_M, '\0', '\0', '\0'},
            {HID_KEY_COMMA, '\0', '\0', '\0'},
            {HID_KEY_PERIOD, '\0', '\0', '\0'},
            {HID_KEY_SLASH, '\0', '\0', '\0'},
            {HID_KEY_SHIFT_RIGHT, HID_KEY_SHIFT_RIGHT, HID_KEY_SHIFT_RIGHT, HID_KEY_SHIFT_RIGHT},
        },
        {
            {HID_KEY_MENU, HID_KEY_ALT_RIGHT, '\0', '\0'},
            {2, 2, 2, 2}, // Modifier key
            {HID_KEY_SPACE, HID_KEY_SPACE, HID_KEY_SPACE, HID_KEY_SPACE},
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
            {'\0', '\0', '\0', '\0'}, // Empty - No key here
        },
    };
    // Call function to put the keys into the keymap variable
    create_keymaps(keys);
  }
};

// Add all of the keys to the keymap variable
void KeyMap::create_keymaps(uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS]) {
  for (int r = 0; r < NUM_ROWS; r++) {
    for (int c = 0; c < NUM_COLUMNS; c++) {
      for (int l = 0; l < NUM_LAYERS; l++) {
        keymap[r][c][l] = keys[r][c][l];
      }
    }
  }
}

bool KeyMap::is_layer_key(uint8_t row, uint8_t column) {
  if (KeyMap::return_keycode(row, column, 0) == 1) {
    return true;
  }
  return false;
}

uint8_t KeyMap::return_keycode(uint8_t row, uint8_t column, uint8_t layer) {
  return keymap[row][column][layer];
}
