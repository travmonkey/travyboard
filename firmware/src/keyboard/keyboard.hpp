// travmonkey

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>

#define LEFT_MOD_ROW_PIN 15
#define LEFT_MOD_COLUMN_PIN 13
#define RIGHT_MOD_ROW_PIN 15
#define RIGHT_MOD_COLUMN_PIN 11

#define DEBOUNCE_TIME 15

static uint8_t current_keys[6];

class KeyBoard {
private:
  bool scan_left_mod(void);
  bool scan_right_mod(void);
  uint8_t set_mod_layer(void);
public:
  constexpr static uint8_t ROW_PINS[2] = {14, 15};
  constexpr static uint8_t COLUMN_PINS[3] = {11, 12, 13};

  KeyBoard(void);
  void scan_buttons(void);
  void handle_button_press();


  static void send_key(bool keys_pressed, uint8_t keys[6]);
};

#endif // KEYBOARD_H
