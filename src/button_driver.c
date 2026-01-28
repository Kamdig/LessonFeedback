#include "button_driver.h"
#include "pico/stdlib.h"

#define BTN_YELLOW_PIN 26
#define BTN_RED_PIN    27
#define BTN_GREEN_PIN  28

#define DEBOUNCE_MS 200

static bool button_pressed(uint pin) {
    return gpio_get(pin) == 0; // aktiv låg
}

void button_driver_init(void) {
    uint pins[] = {
        BTN_RED_PIN,
        BTN_YELLOW_PIN,
        BTN_GREEN_PIN
    };

    for (int i = 0; i < 3; i++) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
    }
}

bool button_driver_was_pressed(button_t button) {
    static absolute_time_t last_press[3] = {0};

    uint pin;
    int index;

    switch (button) {
        case BUTTON_RED:
            pin = BTN_RED_PIN;
            index = 0;
            break;
        case BUTTON_YELLOW:
            pin = BTN_YELLOW_PIN;
            index = 1;
            break;
        case BUTTON_GREEN:
            pin = BTN_GREEN_PIN;
            index = 2;
            break;
        default:
            return false;
    }

    if (button_pressed(pin)) {
        absolute_time_t now = get_absolute_time();
        if (absolute_time_diff_us(last_press[index], now) > DEBOUNCE_MS * 1000) {
            last_press[index] = now;
            return true;
        }
    }
    return false;
}

