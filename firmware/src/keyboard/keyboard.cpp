// travmonkey

#include "keyboard.hpp"
#include "bsp/board.h"
#include "class/hid/hid_device.h"
#include "hardware/gpio.h"
#include "keymap.hpp"
#include "pico/time.h"
#include "usb_descriptors.h"
#include <cstdint>

KeyMap left_keymap("left");
KeyMap right_keymap("right");

KeyBoard::KeyBoard(void) {
  // Initialize the row pins as the output
  for (uint8_t PIN : ROW_PINS) {
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_OUT);
  }

  // Initialize the column pins as the input, and pull them down
  for (uint8_t PIN : COLUMN_PINS) {
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_IN);
    gpio_pull_down(PIN);
  }
}

void KeyBoard::scan_buttons(void) {
  // Poll every 1ms
  const uint32_t interval_ms = 1;
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
  uint8_t layer = set_mod_layer();

  // Loop over every row
  for (uint8_t row = 0; row < sizeof(KeyBoard::ROW_PINS); row++) {
    // Start scanning current row
    gpio_put(KeyBoard::ROW_PINS[row], 1);
    sleep_us(1); // Small delay for accuracy

    // Loop over every column
    for (uint8_t col = 0; col < sizeof(KeyBoard::COLUMN_PINS); col++) {
      if (gpio_get(KeyBoard::COLUMN_PINS[col]) && total_keys < 6) {
        // Scan columns and add the key to the current keys
        current_keys[total_keys] =
            left_keymap.return_keycode(row, col, layer);
        total_keys += 1;
      }
    }

    // Stop scanning current row
    gpio_put(KeyBoard::ROW_PINS[row], 0);
  }

  KeyBoard::handle_button_press();
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

void KeyBoard::handle_button_press() {
  // Handle debouncing
  static uint64_t lastPressTime = 0;
  uint64_t currentTime = time_us_64();

  if (currentTime - lastPressTime < DEBOUNCE_TIME) {
    return;
  }
  lastPressTime = currentTime;

  // Check if either row is recieving input
  for (size_t i = 0; i < 2; i++) {
    if (gpio_get(ROW_PINS[i]) == 0) {
      // Send list of keys pressed
      KeyBoard::send_key(true, current_keys);
    } else {
      KeyBoard::send_key(false, current_keys);
    }
  }
}

void KeyBoard::send_key(bool keys_pressed, uint8_t keys[6]) {
  // skip if hid is not ready yet
  if (!tud_hid_ready()) {
    return;
  }

  // avoid sending multiple zero reports
  static bool send_empty = false;

  if (keys_pressed) {
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keys);
    send_empty = true;
  } else {
    // send empty key report if previously has key pressed
    if (send_empty) {
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
    }
    send_empty = false;
  }
}
