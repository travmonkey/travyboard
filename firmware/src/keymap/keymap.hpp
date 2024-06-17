// travmonkey

#ifndef KEYMAP_H
#define KEYMAP_H

#include <cstdint>
#include <string>

// Define the number of rows and layers
#define NUM_ROWS 4
#define NUM_COLUMNS 7
#define NUM_LAYERS 4

class KeyMap {
private:
  char keymap[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS]; // 3D array to store the keys

public:
  // Constructor allowing both left and right to be defined
  KeyMap(const std::string &name);

  // function to refresh the keyboard
  void refresh();

  void create_keymaps(uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS]);

  uint8_t return_keycode(uint8_t row, uint8_t column, uint8_t layer);

  // List to store the keycodes for the next send
  uint8_t key_codes[6] = {0};
};

#endif // KEYMAP_H
