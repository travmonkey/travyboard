// travmonkey

#ifndef MASTER_H
#define MASTER_H

#include <cstdint>

// Define the mod key pins
#define LEFT_MOD_ROW_PIN 15
#define LEFT_MOD_COLUMN_PIN 13
#define RIGHT_MOD_ROW_PIN 15
#define RIGHT_MOD_COLUMN_PIN 11

// Debounce time to prevent double presses
#define DEBOUNCE_TIME 15

// Uart definitions
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

// Layer functions
uint8_t set_mod_layer();
bool scan_left_mod();
bool scan_right_mod();

void led_blinking_task(void);

#endif // MASTER_H
