#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

#define BTNG 10
#define BTNY 11
#define BTNR 12

void buttons_init(void) {
  gpio_init(BTNG);
  gpio_init(BTNY);
  gpio_init(BTNR);

  gpio_set_dir(BTNG, GPIO_IN);
  gpio_set_dir(BTNY, GPIO_IN);
  gpio_set_dir(BTNR, GPIO_IN);

  gpio_pull_up(BTNG);
  gpio_pull_up(BTNY);
  gpio_pull_up(BTNR);
}

void i2c_init_lcd(void) {
  i2c_init(I2C_PORT, 100 * 1000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SDA_PIN);
  gpio_pull_up(SCL_PIN);
}

#define LCD_ADDR 0x27
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_RS 0x01

void lcd_write_byte(uint8_t val) {
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &val, 1, false);
}

void lcd_toggle_enable(uint8_t val) {
    lcd_write_byte(val | LCD_ENABLE);
    sleep_us(1);
    lcd_write_byte(val & ~LCD_ENABLE);
    sleep_us(50);
}

void lcd_send_nibble(uint8_t nibble, bool rs) {
    uint8_t data = (nibble << 4) | LCD_BACKLIGHT;
    if (rs) data |= LCD_RS;
    lcd_toggle_enable(data);
}

void lcd_send_byte(uint8_t byte, bool rs) {
    lcd_send_nibble(byte >> 4, rs);
    lcd_send_nibble(byte & 0x0F, rs);
}

void lcd_command(uint8_t cmd) {
    lcd_send_byte(cmd, false);
}

void lcd_char(char c) {
    lcd_send_byte(c, true);
}

void lcd_init(void) {
    sleep_ms(50);
    lcd_send_nibble(0x03, false);
    sleep_ms(5);
    lcd_send_nibble(0x03, false);
    sleep_us(150);
    lcd_send_nibble(0x03, false);
    lcd_send_nibble(0x02, false);

    lcd_command(0x28); // 4-bit, 2-line
    lcd_command(0x0C); // display on
    lcd_command(0x06); // entry mode
    lcd_command(0x01); // clear
    sleep_ms(2);
}

void lcd_print(const char *s) {
    while (*s) lcd_char(*s++);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40};
    lcd_command(0x80 | (col + row_offsets[row]));
}

void setup(void) {
  Serial.begin(115200);

  i2c_init_lcd();
  buttons_init();
  lcd_init();

  lcd_command(0x01);
  lcd_set_cursor(0, 0);
  lcd_print("Vad tyckte du");
  lcd_set_cursor(0, 1);
  lcd_print("om lektionen?");

}

static uint8_t green_button_presses = 0;
static uint8_t yellow_button_presses = 0;
static uint8_t red_button_presses = 0;

void loop(void) {
  static bool green_was_pressed = false;
  static bool yellow_was_pressed = false;
  static bool red_was_pressed = false;

  char buf[17];

  bool green_now = !gpio_get(BTNG);
  bool yellow_now = !gpio_get(BTNY);
  bool red_now = !gpio_get(BTNR);

  if (green_now && !green_was_pressed) {
    green_button_presses++;
    lcd_command(0x01);
    snprintf(buf, sizeof(buf), "Green: %d", green_button_presses);
    lcd_print(buf);
  }

  if (yellow_now && !yellow_was_pressed) {
    yellow_button_presses++;
    lcd_command(0x01);
    snprintf(buf, sizeof(buf), "Yellow: %d", yellow_button_presses);
    lcd_print(buf);
  }

  if (red_now && !red_was_pressed) {
    red_button_presses++;
    lcd_command(0x01);
    snprintf(buf, sizeof(buf), "Red: %d", red_button_presses);
    lcd_print(buf);
  }

  green_was_pressed = green_now;
  yellow_was_pressed = yellow_now;
  red_was_pressed = red_now;

  delay(20); // light debounce + CPU mercy
}

