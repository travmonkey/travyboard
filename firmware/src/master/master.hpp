// travmonkey

#ifndef MASTER_H
#define MASTER_H

#include <cstdint>

#define ROW_0 14
#define ROW_1 15
#define COLUMN_0 11
#define COLUMN_1 12
#define COLUMN_2 13

#define DEBOUNCE_TIME 15

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Blink pattern
enum {
  BLINK_NOT_MOUNTED = 250, // device not mounted
  BLINK_MOUNTED = 1000,    // device mounted
  BLINK_SUSPENDED = 2500,  // device is suspended
};

void led_blinking_task(void);

// Keypress functions
void setup(void);
void scan_buttons(void);
void handle_button_press();
void send_key(bool keys_pressed, uint8_t key);

void led_blinking_task(void);

#endif // MASTER_H
