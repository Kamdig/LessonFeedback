
#include <stdio.h>
#include "pico/stdlib.h"
#include "vote_manager.h"
#include "wifi_driver.h"
#include "button_driver.h"
#include "lcd_driver.h"

static void update_lcd(void) {
    VoteResults r = get_results();
    char buf[17];

    lcd_driver_clear();

    snprintf(buf, sizeof(buf), "R:%d Y:%d", r.red, r.yellow);
    lcd_driver_set_cursor(0,0);
    lcd_driver_print(buf);

    snprintf(buf, sizeof(buf), "G:%d", r.green);
    lcd_driver_set_cursor(0,1);
    lcd_driver_print(buf);
}

int main(void) {
    stdio_init_all();

    // WiFi
    if (!wifi_driver_init()) {
        printf("WiFi kunde inte anslutas. Offline-läge.\n");
    } else {
        printf("WiFi anslutet! Pico W är online.\n");
    }

    vote_manager_init();
    button_driver_init();
    lcd_driver_init();

    printf("Vote counter (buttons + LCD)\n");

    while (true) {
        if (button_driver_was_pressed(BUTTON_RED)) {
            add_vote(VOTE_RED);
            printf("Red button pressed\n");
            update_lcd();
        }

        if (button_driver_was_pressed(BUTTON_YELLOW)) {
            add_vote(VOTE_YELLOW);
            printf("Yellow button pressed\n");
            update_lcd();
        }

        if (button_driver_was_pressed(BUTTON_GREEN)) {
            add_vote(VOTE_GREEN);
            printf("Green button pressed\n");
            update_lcd();
        }

        sleep_ms(50); // enkel debounce
    }
}

