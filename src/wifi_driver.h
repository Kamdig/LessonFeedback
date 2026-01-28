#pragma once
#include <stdbool.h>

bool wifi_driver_init(void);

// HTTP GET (host and path), return as true if the request was sent as ok

bool wifi_http_get(const char *host, const char *path);

