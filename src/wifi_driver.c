#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "wifi_driver.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#define DNS_TIMEOUT_MS 10000
#define CONNECT_TIMEOUT_MS 10000
#define POLL_DELAY_MS 10

typedef struct {
    struct tcp_pcb *pcb;
    ip_addr_t addr;
    bool dns_done;
    err_t dns_err;
    bool done;
    bool success;
    bool printed;
    char request[320];
    size_t request_len;
} http_state_t;

static void wifi_dns_found(const char *name, const ip_addr_t *ipaddr, void *arg) {
    http_state_t *state = (http_state_t *)arg;
    (void)name;
    if (ipaddr) {
        state->addr = *ipaddr;
        state->dns_err = ERR_OK;
    } else {
        state->dns_err = ERR_VAL;
    }
    state->dns_done = true;
}

static err_t wifi_tcp_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    http_state_t *state = (http_state_t *)arg;
    if (err != ERR_OK) {
        state->done = true;
        state->success = false;
        return err;
    }

    err = tcp_write(tpcb, state->request, state->request_len, TCP_WRITE_FLAG_COPY);
    if (err != ERR_OK) {
        state->done = true;
        state->success = false;
        return err;
    }

    err = tcp_output(tpcb);
    if (err != ERR_OK) {
        state->done = true;
        state->success = false;
        return err;
    }

    return ERR_OK;
}

static err_t wifi_tcp_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    http_state_t *state = (http_state_t *)arg;

    if (p == NULL) {
        state->done = true;
        if (tcp_close(tpcb) != ERR_OK) {
            tcp_abort(tpcb);
        }
        return ERR_OK;
    }

    if (err != ERR_OK) {
        pbuf_free(p);
        state->done = true;
        state->success = false;
        return err;
    }

    if (!state->printed) {
        char buf[256];
        u16_t copy_len = (p->tot_len < (sizeof(buf) - 1)) ? p->tot_len : (sizeof(buf) - 1);
        pbuf_copy_partial(p, buf, copy_len, 0);
        buf[copy_len] = '\0';
        printf("[WiFi] Response:\n%s\n", buf);
        state->printed = true;
    }

    state->success = true;
    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);
    return ERR_OK;
}

static void wifi_tcp_err(void *arg, err_t err) {
    http_state_t *state = (http_state_t *)arg;
    (void)err;
    state->done = true;
    state->success = false;
}

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

bool wifi_http_get(const char *host, const char *path) {
    if (host == NULL || path == NULL) {
        return false;
    }

    http_state_t state = {0};
    int written = snprintf(
        state.request,
        sizeof(state.request),
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n",
        path,
        host
    );

    if (written < 0 || written >= (int)sizeof(state.request)) {
        printf("[WiFi] ERROR: request too long\n");
        return false;
    }

    state.request_len = (size_t)written;

    cyw43_arch_lwip_begin();
    err_t dns_ret = dns_gethostbyname(host, &state.addr, wifi_dns_found, &state);
    cyw43_arch_lwip_end();

    if (dns_ret == ERR_OK) {
        state.dns_done = true;
        state.dns_err = ERR_OK;
    } else if (dns_ret != ERR_INPROGRESS) {
        printf("[WiFi] ERROR: dns_gethostbyname failed (%d)\n", dns_ret);
        return false;
    }

    absolute_time_t dns_start = get_absolute_time();
    while (!state.dns_done) {
        if (absolute_time_diff_us(dns_start, get_absolute_time()) / 1000 > DNS_TIMEOUT_MS) {
            printf("[WiFi] ERROR: DNS timeout\n");
            return false;
        }
        cyw43_arch_poll();
        sleep_ms(POLL_DELAY_MS);
    }

    if (state.dns_err != ERR_OK) {
        printf("[WiFi] ERROR: DNS failed (%d)\n", state.dns_err);
        return false;
    }

    cyw43_arch_lwip_begin();
    state.pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!state.pcb) {
        cyw43_arch_lwip_end();
        printf("[WiFi] ERROR: tcp_new failed\n");
        return false;
    }

    tcp_arg(state.pcb, &state);
    tcp_recv(state.pcb, wifi_tcp_recv);
    tcp_err(state.pcb, wifi_tcp_err);

    err_t conn_err = tcp_connect(state.pcb, &state.addr, 80, wifi_tcp_connected);
    cyw43_arch_lwip_end();

    if (conn_err != ERR_OK) {
        printf("[WiFi] ERROR: tcp_connect failed (%d)\n", conn_err);
        return false;
    }

    absolute_time_t conn_start = get_absolute_time();
    while (!state.done) {
        if (absolute_time_diff_us(conn_start, get_absolute_time()) / 1000 > CONNECT_TIMEOUT_MS) {
            printf("[WiFi] ERROR: connect timeout\n");
            cyw43_arch_lwip_begin();
            if (state.pcb) {
                tcp_abort(state.pcb);
            }
            cyw43_arch_lwip_end();
            return false;
        }
        cyw43_arch_poll();
        sleep_ms(POLL_DELAY_MS);
    }

    if (!state.success) {
        printf("[WiFi] ERROR: request failed\n");
    }

    return state.success;
}
