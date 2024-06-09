// travmonkey

#include "hardware/gpio.h"
#include "pico/types.h"

#define ENCODER_CLK 9
#define ENCODER_DT 8
#define ENCODER_SW 7

volatile int encoder_pos = 0;
bool last_button_state = true; // Variable to store the last state of the button

void gpio_isr_handler(uint gpio, uint32_t events) {
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

void init_encoder() {
  gpio_init(ENCODER_CLK);
  gpio_set_dir(ENCODER_CLK, GPIO_IN);
  gpio_pull_up(ENCODER_CLK);

  gpio_init(ENCODER_DT);
  gpio_set_dir(ENCODER_DT, GPIO_IN);
  gpio_pull_up(ENCODER_DT);

  gpio_init(ENCODER_SW);
  gpio_set_dir(ENCODER_SW, GPIO_IN);
  gpio_pull_up(ENCODER_SW);

  gpio_set_irq_enabled_with_callback(ENCODER_CLK,
                                     GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
                                     true, &gpio_isr_handler);
}
