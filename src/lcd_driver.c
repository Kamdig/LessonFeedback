#include "lcd_driver.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

#define LCD_ADDR 0x27
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_RS 0x01

static void lcd_write(uint8_t val) {
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &val, 1, false);
}

static void lcd_toggle(uint8_t val) {
    lcd_write(val | LCD_ENABLE);
    sleep_us(1);
    lcd_write(val & ~LCD_ENABLE);
    sleep_us(50);
}

static void lcd_send_nibble(uint8_t nibble, bool rs) {
    uint8_t data = (nibble << 4) | LCD_BACKLIGHT;
    if (rs) data |= LCD_RS;
    lcd_toggle(data);
}

static void lcd_send_byte(uint8_t byte, bool rs) {
    lcd_send_nibble(byte >> 4, rs);
    lcd_send_nibble(byte & 0x0F, rs);
}

void lcd_driver_init(void) {
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    sleep_ms(50);
    lcd_send_nibble(0x03, false);
    sleep_ms(5);
    lcd_send_nibble(0x03, false);
    sleep_us(150);
    lcd_send_nibble(0x02, false);

    lcd_send_byte(0x28, false); // 4-bit, 2-line
    lcd_send_byte(0x0C, false); // display on
    lcd_send_byte(0x06, false); // entry mode
    lcd_send_byte(0x01, false); // clear
    sleep_ms(2);
}

void lcd_driver_clear(void) {
    lcd_send_byte(0x01, false);
    sleep_ms(2);
}

void lcd_driver_set_cursor(uint8_t col, uint8_t row) {
    static const uint8_t offsets[] = {0x00, 0x40};
    lcd_send_byte(0x80 | (col + offsets[row]), false);
}

void lcd_driver_print(const char *text) {
    while (*text) {
        lcd_send_byte(*text++, true);
    }
}

