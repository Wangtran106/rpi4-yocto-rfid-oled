#ifndef SSD1306_I2C_H
#define SSD1306_I2C_H

#include <stdint.h>

#define SSD1306_I2C_ADDR 0x3C

int ssd1306_init(const char *i2c_device);
void ssd1306_clear(void);
void ssd1306_print_string(int x, int y, const char *str);
void ssd1306_close(void);

#endif
