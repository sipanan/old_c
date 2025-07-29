/**
 * @file wifi_manager.hpp
 * @brief Enterprise WiFi Management Interface
 * @version 1.0.0
 * @date 2024
 * 
 * @copyright Copyright (c) 2024
 * @license MIT License
 */

#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

#include "esp_err.h"
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WIFI_STATE_IDLE = 0,
    WIFI_STATE_CONNECTING,
    WIFI_STATE_CONNECTED,
    WIFI_STATE_DISCONNECTED,
    WIFI_STATE_ERROR
} wifi_state_t;

typedef struct {
    char ssid[64];
    char password[64];
    bool auto_connect;
    uint8_t max_retry;
} wifi_config_t;

esp_err_t wifi_manager_init(void);
esp_err_t wifi_manager_connect(const char *ssid, const char *password);
esp_err_t wifi_manager_disconnect(void);
wifi_state_t wifi_manager_get_state(void);
esp_err_t wifi_manager_get_ip_info(char *ip_str, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif // WIFI_MANAGER_HPP