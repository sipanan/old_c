/**
 * @file config_manager.cpp
 * @brief Configuration Management Implementation
 * @version 1.0.0
 * @date 2024
 * 
 * @copyright Copyright (c) 2024
 * @license MIT License
 */

#include "config_manager.hpp"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include <cstring>

static const char *TAG = "CONFIG_MANAGER";
static const char *NVS_NAMESPACE = "qr_scanner";

static nvs_handle_t nvs_handle;
static bool config_initialized = false;

esp_err_t config_manager_init(void)
{
    if (config_initialized) {
        return ESP_OK;
    }
    
    ESP_LOGI(TAG, "Initializing configuration manager");
    
    esp_err_t ret = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS namespace: %s", esp_err_to_name(ret));
        return ret;
    }
    
    config_initialized = true;
    ESP_LOGI(TAG, "Configuration manager initialized");
    
    return ESP_OK;
}

esp_err_t config_manager_save_wifi_config(const char *ssid, const char *password)
{
    if (!config_initialized || !ssid) {
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "Saving WiFi configuration");
    
    esp_err_t ret = nvs_set_str(nvs_handle, "wifi_ssid", ssid);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to save SSID: %s", esp_err_to_name(ret));
        return ret;
    }
    
    if (password) {
        ret = nvs_set_str(nvs_handle, "wifi_pass", password);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to save password: %s", esp_err_to_name(ret));
            return ret;
        }
    }
    
    ret = nvs_commit(nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to commit WiFi config: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "WiFi configuration saved");
    }
    
    return ret;
}

esp_err_t config_manager_load_wifi_config(char *ssid, size_t ssid_len, char *password, size_t pass_len)
{
    if (!config_initialized || !ssid) {
        return ESP_ERR_INVALID_STATE;
    }
    
    size_t required_size = ssid_len;
    esp_err_t ret = nvs_get_str(nvs_handle, "wifi_ssid", ssid, &required_size);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "No saved SSID found: %s", esp_err_to_name(ret));
        return ret;
    }
    
    if (password && pass_len > 0) {
        required_size = pass_len;
        ret = nvs_get_str(nvs_handle, "wifi_pass", password, &required_size);
        if (ret != ESP_OK) {
            ESP_LOGW(TAG, "No saved password found: %s", esp_err_to_name(ret));
            password[0] = '\0'; // Empty password
        }
    }
    
    ESP_LOGI(TAG, "WiFi configuration loaded: SSID=%s", ssid);
    return ESP_OK;
}