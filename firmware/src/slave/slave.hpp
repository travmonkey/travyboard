// travmonkey

#ifndef SLAVE_H
#define SLAVE_H

// Define the mod key pins
#define LEFT_MOD_ROW_PIN 15
#define LEFT_MOD_COLUMN_PIN 13
#define RIGHT_MOD_ROW_PIN 15
#define RIGHT_MOD_COLUMN_PIN 11

// Debounce time to prevent double presses
#define DEBOUNCE_TIME 15

// Uart definitions
#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Blink pattern
enum {
  BLINK_NOT_MOUNTED = 250, // device not mounted
  BLINK_MOUNTED = 1000,    // device mounted
  BLINK_SUSPENDED = 2500,  // device is suspended
};

void led_blinking_task(void);

#endif // SLAVE_H
