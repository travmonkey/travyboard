#include "pico/stdio.h"
#include "pico/time.h"
#include <cstdio>
#include <cstring>

#define UART_ID uart0
#define TX_PIN 0
#define RX_PIN 1
#define BAUD_RATE 115200

// // Function to initialize UART
// void init_uart() {
//   uart_init(UART_ID, BAUD_RATE);
//
//   gpio_set_function(TX_PIN, GPIO_FUNC_UART);
//   gpio_set_function(RX_PIN, GPIO_FUNC_UART);
// }
//
// // Function to receive and process messages
// void receive_messages() {
//   while (true) {
//     while (uart_is_readable(UART_ID)) {
//       char received_char = uart_getc(UART_ID);
//       if (received_char == '\n') {
//         printf("\n"); // Print newline for easier message separation
//       } else {
//         printf("%c", received_char);
//       }
//     }
//   }
// }

int main() {
  stdio_init_all();
  // init_uart();

  // receive_messages();
  while (true) {
    printf("Hello!\n");
    sleep_ms(1000);
  }

  return 0;
}
