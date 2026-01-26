#include "thingspeak_driver.h"
#include "thingspeak_driver.c"
#include "wifi_driver.h"

#include "pico/stdlib.h"
#include <stdio.h>
#include <stdbool.h>

bool send_vote_to_thingspeak(int lessonID, int voteValue)
{
    static absolute_time_t last_send_time;
    static bool has_sent_once = false;

    if (has_sent_once) {
        int64_t elapsed_ms =
            absolute_time_diff_us(last_send_time, get_absolute_time()) / 1000;

        if (elapsed_ms < 30000) {
            printf("[ThingSpeak] Skickar inte igen ännu (%lld ms sedan sist)\n", elapsed_ms);
            return false;
           
        }

    }
    
    char path[200];
    int written = snprintf(
        path,
        sizeof(path),
        "/update?api_key=%s&field1=%d&field2=%d",
        THINGSPEAK_API_KEY,
        lessondID,
        voteValue
    );

    if (written < 0 || written >= (int)sizeof(path)) {
        printf("[ThingSpeak] ERROR: URL för lång\n");
        return false; 

    }  
   
    bool ok = wifi_http_get("api.thingspeak.com", path);
    if (ok) {
        last_send_time = get_absolute_time();
        has_sent_once = true;
        printf("[ThingSpeak] OK\n");
        return true;
    
    }

    printf("[ThingSpeak] FAIL\n");
    return false;      

}
