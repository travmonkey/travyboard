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
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Left and Right half definitions, out of index for possible key array
#define LEFT_HALF 253
#define RIGHT_HALF 254

// LEGENDARY JADEN CROW SOLUTION
struct KeyPress {
  uint8_t row;
  uint8_t col;

  KeyPress() : row(NULL_VALUE), col(NULL_VALUE) {}
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
  uint8_t set_mod_layer(void);

  void handle_uart(void);

  void button_handler(uint8_t row, uint8_t col, char half);

  void send_keyboard_report(uint8_t layer);

  uint8_t check_layer(void);

  void refresh(void);

  constexpr static uint8_t LEFT_ROW_PINS[2] = {14, 15};
  constexpr static uint8_t LEFT_COLUMN_PINS[3] = {11, 12, 13};
  constexpr static uint8_t RIGHT_ROW_PINS[2] = {14, 15};
  constexpr static uint8_t RIGHT_COLUMN_PINS[3] = {11, 12, 13};

public:
  KeyBoard(const std::int8_t &name);
  void scan_buttons(void);
  Keys scan_pins(void);
};

#endif // KEYBOARD_H
