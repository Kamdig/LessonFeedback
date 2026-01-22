#include "wifi_driver.h"
#include <stdio.h>
#include "pico/cyw43_arch.h"

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

bool wifi_connect(void) {
    printf("Initierar WiFi...\n");

    if (cyw43_arch_init()) {
        printf("WiFi init misslyckades\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();

    printf("Ansluter till WiFi: %s\n", WIFI_SSID);

    int result = cyw43_arch_wifi_connect_timeout_ms(
        WIFI_SSID,
        WIFI_PASSWORD,
        CYW43_AUTH_OPEN,
        30000
    );

    if (result != 0) {
        printf("WiFi-anslutning misslyckades\n");
        return false;
    }

    printf("WiFi anslutet!\n");
    return true;
}

