// travmonkey

#include "keyboard.hpp"
#include "bsp/board.h"
#include "class/hid/hid_device.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "keymap.hpp"
#include "pico/time.h"
#include "usb_descriptors.h"
#include <cstdint>

KeyMap left_keymap("left");
KeyMap right_keymap("right");

KeyBoard::KeyBoard(const std::string &name) {
  if (name == "right") {
    // Initialize right half of keyboard
    // Initialize the row pins as the output
    for (uint8_t PIN : RIGHT_ROW_PINS) {
      gpio_init(PIN);
      gpio_set_dir(PIN, GPIO_OUT);
    }
    // Initialize the column pins as the input, and pull them down
    for (uint8_t PIN : RIGHT_COLUMN_PINS) {
      gpio_init(PIN);
      gpio_set_dir(PIN, GPIO_IN);
      gpio_pull_down(PIN);
    }
  } else if (name == "left") {
    // Initialize left half of keyboard
    // Initialize the row pins as the output
    for (uint8_t PIN : LEFT_ROW_PINS) {
      gpio_init(PIN);
      gpio_set_dir(PIN, GPIO_OUT);
    }
    // Initialize the column pins as the input, and pull them down
    for (uint8_t PIN : LEFT_COLUMN_PINS) {
      gpio_init(PIN);
      gpio_set_dir(PIN, GPIO_IN);
      gpio_pull_down(PIN);
    }
  }
}

void KeyBoard::scan_buttons() {
  // Poll every 1ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  // Check for time since last poll
  if (board_millis() - start_ms < interval_ms) {
    return; // not enough time
  }
  start_ms += interval_ms;

  // Reset the current_keys array
  for (size_t i = 0; i < 6; i++) {
    current_keys[i] = 0;
  }

  // Define the total amount of keys pressed. This is a max of 6
  uint total_keys = 0;

  // Set the current layer
  // uint8_t layer = set_mod_layer();
  uint8_t layer = 0;

  // Loop over every row
  for (uint8_t row = 0; row < sizeof(KeyBoard::RIGHT_ROW_PINS); row++) {
    // Start scanning current row
    gpio_put(KeyBoard::RIGHT_ROW_PINS[row], 1);
    sleep_us(1); // Small delay for accuracy

    // Loop over every column
    for (uint8_t col = 0; col < sizeof(KeyBoard::RIGHT_COLUMN_PINS); col++) {
      if (gpio_get(KeyBoard::RIGHT_COLUMN_PINS[col]) && total_keys < 6) {
        // Scan columns and add the key to the current keys
        current_keys[total_keys] = right_keymap.return_keycode(row, col, layer);
        total_keys += 1;
      }
    }
    // Stop scanning current row
    gpio_put(KeyBoard::RIGHT_ROW_PINS[row], 0);
  }

  // While loop to recieve everything in buffer, or until max keystrokes is hit
  while (uart_is_readable(UART_ID) && total_keys < 6) {
    // Recieve first in the buffer from uart
    uint8_t packet = uart_getc(UART_ID);
    uint8_t row = (packet >> 4); // Bitshift to get row value
    uint8_t col = (packet & 0x0f); // & 0x0f returns the col value

    uint8_t keycode = left_keymap.return_keycode(row, col, layer); // set keycode in variable

    // Loop to ensure that the same key doesn't get added twice into single send
    bool found = false;
    for (uint8_t code : current_keys) {
      if (code == keycode) {
        found = true;
        break;
      }
    }

    // Add the key to the current_keys array
    if (!found) {
      current_keys[total_keys] = keycode;
      total_keys++;
    }
  }

  // Send the array of keypresses
  if (tud_hid_ready()) {
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, current_keys);
    // Make sure memory is completely clear just in case
    memset(current_keys, 0, sizeof(current_keys));
  }
}

Keys KeyBoard::scan_pins(void) {
  // Poll every 1ms
  const uint32_t interval_ms = 5;
  static uint32_t start_ms = 0;

  Keys keys;

  // Check for time since last poll
  if (board_millis() - start_ms < interval_ms) {
    return keys; // not enough time
  }
  start_ms += interval_ms;

  // Define the total amount of keys pressed. This is a max of 6
  uint total_keys = 0;

  // Set the current layer
  uint8_t layer = 0;
  // uint8_t layer = set_mod_layer();

  // Loop over every row
  for (uint8_t row = 0; row < sizeof(KeyBoard::LEFT_ROW_PINS); row++) {
    // Start scanning current row
    gpio_put(KeyBoard::LEFT_ROW_PINS[row], 1);
    sleep_us(1); // Small delay for accuracy

    // Loop over every column
    for (uint8_t col = 0; col < sizeof(KeyBoard::LEFT_COLUMN_PINS); col++) {
      if (gpio_get(KeyBoard::LEFT_COLUMN_PINS[col]) && total_keys < 6) {
        // Scan columns and add the key to the current keys
        keys[total_keys].row = row;
        keys[total_keys].col = col;
        keys[total_keys].layer = layer;
        total_keys += 1;
      }
    }

    // Stop scanning current row
    gpio_put(KeyBoard::LEFT_ROW_PINS[row], 0);
  }

  return keys;
}

bool KeyBoard::scan_left_mod(void) {
  // Scan for left layer key
  gpio_put(LEFT_MOD_ROW_PIN, 1);
  sleep_us(1); // Small delay for accuracy
  bool is_pressed = gpio_get(LEFT_MOD_COLUMN_PIN);
  gpio_put(LEFT_MOD_ROW_PIN, 0);
  return is_pressed;
}

bool KeyBoard::scan_right_mod(void) {
  // Scan for left layer key
  gpio_put(RIGHT_MOD_ROW_PIN, 1);
  sleep_us(1); // Small delay for accuracy
  bool is_pressed = gpio_get(RIGHT_MOD_COLUMN_PIN);
  gpio_put(RIGHT_MOD_ROW_PIN, 0);
  return is_pressed;
}

// Potentially rework this with less scans if latency becomes an issue
uint8_t KeyBoard::set_mod_layer(void) {
  // Check which layer keys are pressed
  if (scan_left_mod() && scan_right_mod()) {
    return 3; // Return layer 3 if both pressed
  } else if (scan_left_mod()) {
    return 1; // Return layer 1 if only left pressed
  } else if (scan_right_mod()) {
    return 2; // Return layer 2 if only right pressed
  }
  return 0; // Return default (0) layer
};
