#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include <esp_err.h>

#define MAX_TIMEOUT_MS 10000
esp_err_t wifi_init(void);

#endif