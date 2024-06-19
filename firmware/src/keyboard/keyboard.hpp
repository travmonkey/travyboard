// travmonkey

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>
#include <string>

#define LEFT_MOD_ROW_PIN 15
#define LEFT_MOD_COLUMN_PIN 13
#define RIGHT_MOD_ROW_PIN 15
#define RIGHT_MOD_COLUMN_PIN 11

#define NULL_VALUE 255

#define DEBOUNCE_TIME 15

// Uart definitions
#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static uint8_t current_keys[6];
static int total_keys;

// LEGENDARY JADEN CROW SOLUTION
struct KeyPress {
  uint8_t row;
  uint8_t col;
  uint8_t layer;

  KeyPress() : row(NULL_VALUE), col(NULL_VALUE), layer(NULL_VALUE) {}
};

struct Keys {
  KeyPress location[6];
  Keys() {
    for (int i = 0; i < 6; i++) {
      location[i] = KeyPress();
    }
  }
  KeyPress &operator[](int i) { return location[i]; };
};

class KeyBoard {
private:
  bool scan_left_mod(void);
  bool scan_right_mod(void);

  uint8_t set_mod_layer(void);

  void handle_uart(void);

  constexpr static uint8_t LEFT_ROW_PINS[2] = {14, 15};
  constexpr static uint8_t LEFT_COLUMN_PINS[3] = {11, 12, 13};
  constexpr static uint8_t RIGHT_ROW_PINS[2] = {14, 15};
  constexpr static uint8_t RIGHT_COLUMN_PINS[3] = {11, 12, 13};

public:

  KeyBoard(const std::string &name);
  void scan_buttons(void);
  Keys scan_pins(void);

};

#endif // KEYBOARD_H
