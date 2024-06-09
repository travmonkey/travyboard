#ifndef ENCODER_H
#define ENCODER_H

#include "pico/types.h"

namespace Encoder {
namespace {
const uint ENCODER_CLK = 9;
const uint ENCODER_DT = 8;
const uint ENCODER_SW = 7;
volatile int encoder_pos = 0;
// extern bool last_button_state;
} // namespace

void gpio_isr_handler(uint gpio, uint32_t events);
void init_encoder();
int get_position();
void reset_position();
bool button_clicked();

// Add more function declarations here
} // namespace Encoder

#endif // ENCODER_H
