/**
 * @file config_manager.hpp
 * @brief Configuration Management Interface
 * @version 1.0.0
 * @date 2024
 * 
 * @copyright Copyright (c) 2024
 * @license MIT License
 */

#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t config_manager_init(void);
esp_err_t config_manager_save_wifi_config(const char *ssid, const char *password);
esp_err_t config_manager_load_wifi_config(char *ssid, size_t ssid_len, char *password, size_t pass_len);

#ifdef __cplusplus
}
#endif

#endif // CONFIG_MANAGER_HPP