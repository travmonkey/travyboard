#include "oled.hpp"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include <cstdint>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 23

OledDisplay::OledDisplay(i2c_inst_t *i2c, uint sda, uint scl)
    : i2c(i2c), SDA(sda), SCL(scl) {}

void OledDisplay::init() {
  i2c_init(i2c, 400 * 1000);
  gpio_set_function(SDA, GPIO_FUNC_I2C);
  gpio_set_function(SCL, GPIO_FUNC_I2C);
  gpio_pull_up(SDA);
  gpio_pull_up(SCL);

  uint8_t init_cmds[] = {0xAE,       // Display off
                         0x00, 0x10, // Set lower and higher column address
                         0x00,       // Set display start line
                         0xB0,       // Set page address
                         0x81,       // Contract Control
                         0xCF,       // 128
                         0xA1,       // Set segment remap
                         0xA6,       // Normal / Reverse
                         0xA8,       // Multiplex Ratio
                         0x1F,       // Duty = 1/32
                         0xC8,       // Com scan direction
                         0xD3, 0x00, // Set display offset
                         0xD5, 0x80, // Set osc division
                         0xD9, 0x1F, // Set pre-charge period
                         0xDA, 0x00, // Set com pins
                         0xDB, 0x40, // Set Vcomh deselect level
                         0x8D, 0x10, // Set charge pump enable
                         0xAF};      // Display on

  for (uint8_t cmd : init_cmds) {
    write_cmd(cmd);
  }
}

uint8_t find_oled_addr(i2c_inst_t *i2c) {
  for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
    int result = i2c_write_blocking(i2c, addr, NULL, 0, false);
    if (result >= 0) {
      gpio_put(0, true);
      return addr; // Found a device, return its address
    }
  }
  return 0; // No device found
}

void OledDisplay::write_cmd(uint8_t cmd) {
  uint8_t buf[2] = {0x00, cmd};
  i2c_write_blocking(i2c, 0x3C, buf, 2, false);
}

void OledDisplay::clear() {
  for (uint16_t i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT / 8); i++) {
    write_data(0x00);
  }
}

void OledDisplay::write_data(uint8_t data) {
  uint8_t buf[2] = {0x40, data};
  i2c_write_blocking(i2c, 0x3C, buf, 2, false);
}

void OledDisplay::set_cursor(uint8_t col, uint8_t row) {
  write_cmd(0x21);
  write_cmd(col);
  write_cmd(SCREEN_WIDTH - 1);
  write_cmd(0x22);
  write_cmd(row);
  write_cmd((SCREEN_HEIGHT / 8) - 1);
}

void OledDisplay::draw_text(const char *text) {
  set_cursor(0, 0);
  while (*text) {
    write_data(*text++);
  }
}
