#ifndef OLED_H
#define OLED_H

#include "hardware/i2c.h"
#include "pico/types.h"
#include <cstdint>

class OledDisplay {
public:
  OledDisplay(i2c_inst_t *i2c, uint sda, uint scl);
  void init();
  void clear();
  void draw_text(const char *text);

private:
  i2c_inst_t *i2c;
  uint8_t addr;
  uint SDA;
  uint SCL;

  void write_cmd(uint8_t cmd);
  void write_data(uint8_t data);
  void set_cursor(uint8_t col, uint8_t row);
};

#endif // OLED_H
