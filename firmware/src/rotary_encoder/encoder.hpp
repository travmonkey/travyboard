#ifndef ENCODER_H
#define ENCODER_H

#include "pico/types.h"

class RotaryEncoder {
public:
  RotaryEncoder(uint clk, uint dt, uint sw);
  int get_position();
  void reset_position();
  bool button_clicked();
  void listen();

private:
  const uint ENCODER_CLK;
  const uint ENCODER_DT;
  const uint ENCODER_SW;
  void handle_interrupt(uint gpio, uint32_t events);
  static void gpio_isr_handler(uint gpio, uint32_t events);
  static RotaryEncoder *instance;
  volatile int encoder_pos = 0;
};

// Add more function declarations here

#endif // ENCODER_H
