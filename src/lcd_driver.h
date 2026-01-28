#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

void lcd_driver_init(void);
void lcd_driver_clear(void);
void lcd_driver_set_cursor(uint8_t col, uint8_t row);
void lcd_driver_print(const char *text);

#endif

