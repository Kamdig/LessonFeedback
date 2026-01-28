#pragma once
#include <stdbool.h>

typedef enum {
    BUTTON_RED,
    BUTTON_YELLOW,
    BUTTON_GREEN
} button_t;

void button_driver_init(void);
bool button_driver_was_pressed(button_t button);

