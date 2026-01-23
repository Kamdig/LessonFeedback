#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "wifi_driver.h"

bool wifi_driver_init(void) {
    if (cyw43_arch_init()) {
        printf("WiFi init misslyckades\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();

    printf("Ansluter till WiFi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(
            "Wokwi-GUEST",
            "",
            CYW43_AUTH_OPEN,
            30000)) {
        printf("WiFi-anslutning misslyckades\n");
        return false;
    }

    printf("WiFi ansluten!\n");
    return true;
}

