#include <stdio.h>
#include "pico/stdlib.h"
#include "vote_manager.h"
#include "wifi_driver.h"
#include "button_driver.h"
#include "lcd_driver.h"
#include "thingspeak_driver.h"

#define LESSON_ID 1 // Välj ID för lektionen

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
// Skapar en funktion som lagrar rösten och skickar sedan direkt till molnet
static void handle_vote(int voteValue) {
   
    // Spara rösten lokalt
    add_vote(voteValue);

    // Skicka den vidare tillsammans med lektions ID till ThingSpeak 
    (void)send_vote_to_thingspeak(LESSON_ID, voteValue);
    
    // Skriv ut meddelande och uppdatera LCD:n
    printf("Tack för din röst\n");
    update_lcd();

}

int main(void) {
    stdio_init_all();

    // Innan vi anropar funktionerna
    // Anslut Pico W till WiFi
    if (!wifi_driver_init()) {
        printf("WiFi kunde inte anslutas. Offline-läge.\n");
    } else {
        printf("WiFi anslutet! Pico W är online.\n");
    }
    // initiera funktioner för rösthantering, knappar och LCD
    vote_manager_init();
    button_driver_init();
    lcd_driver_init();

    printf("Vote counter (buttons + LCD)\n");

    while (true) {
        if (button_driver_was_pressed(BUTTON_RED)) {
            handle_vote(VOTE_RED);
            printf("Red button pressed\n");
        }

        if (button_driver_was_pressed(BUTTON_YELLOW)) {
            handle_vote(VOTE_YELLOW);
            printf("Yellow button pressed\n");
        }

        if (button_driver_was_pressed(BUTTON_GREEN)) {
            handle_vote(VOTE_GREEN);
            printf("Green button pressed\n");
        }
     
        // förhindra att flera röster registreras vid ett knapptryck
        sleep_ms(50); 
    }

}

