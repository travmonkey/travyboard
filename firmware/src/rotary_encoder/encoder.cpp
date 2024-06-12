// travmonkey

#include "encoder.hpp"
#include "bsp/board.h"
#include "class/hid/hid_device.h"
#include "hardware/gpio.h"
#include "pico/types.h"
#include "usb_descriptors.h"
#include <cstdint>

RotaryEncoder::RotaryEncoder(uint clk, uint dt, uint sw)
    : ENCODER_CLK(clk), ENCODER_DT(dt), ENCODER_SW(sw) {

  // Create self variable for isr handling
  instance = this;

  // Initialize encoder pins and pull them up
  gpio_init(ENCODER_CLK);
  gpio_set_dir(ENCODER_CLK, GPIO_IN);
  gpio_pull_up(ENCODER_CLK);

  gpio_init(ENCODER_DT);
  gpio_set_dir(ENCODER_DT, GPIO_IN);
  gpio_pull_up(ENCODER_DT);

  gpio_init(ENCODER_SW);
  gpio_set_dir(ENCODER_SW, GPIO_IN);
  gpio_pull_up(ENCODER_SW);

  // Initialize callback handling
  gpio_set_irq_enabled_with_callback(ENCODER_CLK,
                                     GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
                                     true, &RotaryEncoder::gpio_isr_handler);
};

RotaryEncoder *RotaryEncoder::instance = nullptr;

// Send current instance to handle interrupts
void RotaryEncoder::gpio_isr_handler(uint gpio, uint32_t events) {
  if (instance) {
    instance->handle_interrupt(gpio, events);
  }
}

// Handle rotary encoder interrupts
void RotaryEncoder::handle_interrupt(uint gpio, uint32_t events) {
  static int last_encoded = 0;
  static const int enc_states[] = {0,  -1, 1, 0, 1, 0, 0,  -1,
                                   -1, 0,  0, 1, 0, 1, -1, 0};

  int MSB = gpio_get(ENCODER_CLK);
  int LSB = gpio_get(ENCODER_DT);
  int encoded = (MSB << 1) | LSB;
  int sum = (last_encoded << 2) | encoded;
  encoder_pos += enc_states[sum & 0b1111];
  last_encoded = encoded;
}

void RotaryEncoder::listen() {
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if (board_millis() - start_ms < interval_ms) {
    return; // not enough time
  }
  start_ms += interval_ms;

  if (!tud_hid_ready()) {
    return;
  }

  uint8_t current_dir[6] = {0};

  if (encoder_pos > 0) {
    current_dir[0] = 0x80;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, current_dir);
  } else if (encoder_pos < 0) {
    current_dir[0] = 0x81;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, current_dir);
  }
}

// Function to return encoder position
int RotaryEncoder::get_position() { return encoder_pos; }

// Function to reset encoder position
void RotaryEncoder::reset_position() { encoder_pos = 0; }

// Function to return button clicks on encoder
bool RotaryEncoder::button_clicked() {
  if (gpio_get(ENCODER_SW) == 0) {
    return true;
  } else {
    return false;
  }
}

// Add more functions here
