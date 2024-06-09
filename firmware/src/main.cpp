/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board.h"
#include "class/hid/hid.h"
#include "class/hid/hid_device.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "tusb.h"

#include "keyboard.hpp"
#include "usb_descriptors.h"

#define ROW_1 14
#define ROW_2 15
#define COLUMN_1 11
#define COLUMN_2 12
#define COLUMN_3 13

#define DEBOUNCE_TIME 15

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

// Blink pattern
enum {
  BLINK_NOT_MOUNTED = 250, // device not mounted
  BLINK_MOUNTED = 1000,    // device mounted
  BLINK_SUSPENDED = 2500,  // device is suspended
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
static uint8_t current_keys[6] = {0};

void led_blinking_task(void);
void hid_task(void);

void setup(void);
void scanButtons(void);
void handleButtonPress();
void send_key(bool keys_pressed, uint8_t key);

KeyBoard keyboard;

/*------------- MAIN -------------*/
int main(void) {
  setup();
  board_init();
  tusb_init();

  gpio_init(0);
  gpio_set_dir(0, GPIO_OUT);

  while (1) {
    tud_task(); // tinyusb device task

    gpio_put(0, 0);

    led_blinking_task();
    scanButtons();
    // hid_task(); // keyboard implementation
  }

  return 0;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

void setup(void) {
  // Initialize the row pins as the output
  gpio_init(ROW_1);
  gpio_set_dir(ROW_1, GPIO_OUT);
  gpio_init(ROW_2);
  gpio_set_dir(ROW_2, GPIO_OUT);

  // Initialize the column pins as the input, and pull them down
  gpio_init(COLUMN_1);
  gpio_set_dir(COLUMN_1, GPIO_IN);
  gpio_pull_down(COLUMN_1);

  gpio_init(COLUMN_2);
  gpio_set_dir(COLUMN_2, GPIO_IN);
  gpio_pull_down(COLUMN_2);

  gpio_init(COLUMN_3);
  gpio_set_dir(COLUMN_3, GPIO_IN);
  gpio_pull_down(COLUMN_3);
}

void scanButtons(void) {
  // Poll every 1ms
  const uint32_t interval_ms = 1;
  static uint32_t start_ms = 0;

  if (board_millis() - start_ms < interval_ms) {
    return; // not enough time
  }
  start_ms += interval_ms;

  for (size_t i = 0; i < 6; i++) {
    current_keys[i] = 0;
  }

  // Define the total amount of keys pressed. This is a max of 6
  int total_keys = 0;

  // Start scanning row 1
  gpio_put(ROW_1, 1);
  sleep_us(1); // Small delay for accuracy
  // Check each column for changes
  if (gpio_get(COLUMN_1) && total_keys < 6) {
    current_keys[total_keys] = HID_KEY_1;
    total_keys += 1;
  }
  if (gpio_get(COLUMN_2)) {
    current_keys[total_keys] = HID_KEY_ALT_LEFT;
    total_keys += 1;
  }
  if (gpio_get(COLUMN_3)) {
    current_keys[total_keys] = HID_KEY_2;
    total_keys += 1;
  }
  // Stop scanning row 1
  gpio_put(ROW_1, 0);

  // Start scanning row 2
  gpio_put(ROW_2, 1);
  sleep_us(1); // Small delay for accuracy
  // Check each column for changes
  if (gpio_get(COLUMN_1)) {
    current_keys[total_keys] = HID_KEY_A;
    total_keys += 1;
  }
  if (gpio_get(COLUMN_2)) {
    current_keys[total_keys] = HID_KEY_S;
    total_keys += 1;
  }
  if (gpio_get(COLUMN_3)) {
    current_keys[total_keys] = HID_KEY_D;
    total_keys += 1;
  }
  // Stop scanning row 2
  gpio_put(ROW_2, 0);
  handleButtonPress();
}

static void send_key(bool keys_pressed, uint8_t keys[6]) {
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

void handleButtonPress() {
  // Handle debouncing
  static uint64_t lastPressTime = 0;
  uint64_t currentTime = time_us_64();

  if (currentTime - lastPressTime < DEBOUNCE_TIME) {
    return;
  }
  lastPressTime = currentTime;

  // Check if either row is recieving input
  static uint8_t input_pins[2] = {14, 15};
  for (size_t i = 0; i < 2; i++) {
    if (gpio_get(input_pins[i]) == 0) {
      // Send list of keys pressed
      send_key(true, current_keys);
    } else {
      send_key(false, current_keys);
    }
  }
}

static void send_hid_report(bool keys_pressed) {
  // skip if hid is not ready yet
  if (!tud_hid_ready()) {
    return;
  }

  // avoid sending multiple zero reports
  static bool send_empty = false;

  if (keys_pressed) {
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keyboard.key_codes);
    send_empty = true;
  } else {
    // send empty key report if previously has key pressed
    if (send_empty) {
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
    }
    send_empty = false;
  }
}

// Every 10ms, we poll the pins and send a report
void hid_task(void) {
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if (board_millis() - start_ms < interval_ms) {
    return; // not enough time
  }
  start_ms += interval_ms;

  // Check for keys pressed
  bool const keys_pressed = keyboard.update();

  // Remote wakeup
  if (tud_suspended() && keys_pressed) {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  } else {
    // send a report
    send_hid_report(keys_pressed);
  }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report,
                                uint8_t len) {
  // not implemented, we only send REPORT_ID_KEYBOARD
  (void)instance;
  (void)len;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
  // TODO not Implemented
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
  (void)instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT) {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD) {
      // bufsize should be (at least) 1
      if (bufsize < 1)
        return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        board_led_write(true);
      } else {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) { blink_interval_ms = BLINK_MOUNTED; }

// Invoked when device is unmounted
void tud_umount_cb(void) { blink_interval_ms = BLINK_NOT_MOUNTED; }

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
  (void)remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) { blink_interval_ms = BLINK_MOUNTED; }

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void) {
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // blink is disabled
  if (!blink_interval_ms)
    return;

  // Blink every interval ms
  if (board_millis() - start_ms < blink_interval_ms)
    return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
