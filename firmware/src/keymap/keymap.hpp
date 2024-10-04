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
  KeyMap(const std::uint8_t &name);

  // Function to create the keymap array from the constructor
  void create_keymaps(uint8_t keys[NUM_ROWS][NUM_COLUMNS][NUM_LAYERS]);

  // Return the keycode
  uint8_t return_keycode(uint8_t row, uint8_t column, uint8_t layer);

  bool is_layer_key(uint8_t row, uint8_t column);
};

#endif // KEYMAP_H
