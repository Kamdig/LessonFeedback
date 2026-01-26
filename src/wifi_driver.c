#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "wifi_driver.h"

#include "lwip/sockets.h"
#include "lwip/netdb.h"


bool wifi_driver_init() {
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

// GET HTTP
bool wifi_http_get(const char *host, const char *path)
{    
    if (host == NULL || path == NULL) {
        return false;
    }

    char request[320];
    int written = snprintf(
        request,
        sizeof(request),
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n",
        path,
        host
    );

    if (written < 0 || written >= (int)sizeof(request)) {
        printf("[WiFi] ERROR: request too long\n");
        return false;
    
    }

    // DNS lookup
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    struct addrinfo *res = NULL;
    
    cyw43_arch_lwip_begin();
    int err = getaddrinfo(host, "80", &hints, &res);
    cyw43_arch_lwip_end();

    if (err != 0 || res == NULL) {
        printf("[WiFi] ERROR: getaddrinfo failed (%d)\n", err);
        return false;
    
    }
     
    // Create Socket
    cyw43_arch_lwip_begin();
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    cyw43_arch_lwip_end();

    if (sock < 0){
        printf("[WiFi] ERROR: socket() failed\n");
        freeaddrinfo(res);
        return false;
    }
  
    // Connect
    cyw43_arch_lwip_begin();
    int conn = connect(sock, res->ai_addr, res->ai_addrlen);
    cyw43_arch_lwip_end();

    freeaddrinfo(res);
 
    if (conn != 0) {
        printf("[WiFi] ERROR: connect() failed\n");
        cyw43_arch_lwip_begin();
        close(sock);
        cyw43_arch_lwip_end();
        return false;
    
    }

    // Send
    cyw43_arch_lwip_begin();
    int sent = send(sock, request, (int)strlen(request), 0);
    cyw43_arch_lwip_end();

    if (sent < 0) {
        printf("[WiFi] ERROR: send() failed\n");
        cyw43_arch_lwip_begin();
        close(sock);
        cyw43_arch_lwip_end();
        return false;
    }

    // Read first chunk (debug)
    char buf[256];
    
    cyw43_arch_lwip_begin();
    int recvd = recv(sock, buf, (int)sizeof(buf) - 1, 0);
    cyw43_arch_lwip_end();

    if (recvd > 0) {
        buf[recvd] = '\0';
        printf("[WiFi] Response:\n%s\n", buf);
       
    } else {
        printf("[WiFi] No response\n");

    }
     
    cyw43_arch_lwip_begin();
    close(sock);
    cyw43_arch_lwip_end();

    return (recvd > 0);
}
