/**
 * @file utils.cpp
 * @brief Utility Functions Implementation
 * @version 1.0.0
 * @date 2024
 * 
 * @copyright Copyright (c) 2024
 * @license MIT License
 */

#include "utils.hpp"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include <cstdio>

static const char *TAG = "UTILS";

void utils_trigger_memory_recovery(void)
{
    ESP_LOGW(TAG, "Triggering memory recovery procedures");
    
    // Force garbage collection of unused heap
    heap_caps_check_integrity_all(true);
    
    // Log memory status
    size_t free_heap = esp_get_free_heap_size();
    size_t min_heap = esp_get_minimum_free_heap_size();
    
    ESP_LOGW(TAG, "Memory recovery - Free: %zu, Min: %zu", free_heap, min_heap);
    
    // In a critical situation, could trigger controlled restart
    if (free_heap < 5120) { // Less than 5KB
        ESP_LOGE(TAG, "Critical memory situation - system may need restart");
    }
}

esp_err_t utils_format_uptime(uint32_t seconds, char *buffer, size_t buffer_size)
{
    if (!buffer || buffer_size == 0) {
        return ESP_ERR_INVALID_ARG;
    }
    
    uint32_t days = seconds / 86400;
    uint32_t hours = (seconds % 86400) / 3600;
    uint32_t minutes = (seconds % 3600) / 60;
    uint32_t secs = seconds % 60;
    
    if (days > 0) {
        snprintf(buffer, buffer_size, "%lud %02luh %02lum %02lus", days, hours, minutes, secs);
    } else if (hours > 0) {
        snprintf(buffer, buffer_size, "%02luh %02lum %02lus", hours, minutes, secs);
    } else if (minutes > 0) {
        snprintf(buffer, buffer_size, "%02lum %02lus", minutes, secs);
    } else {
        snprintf(buffer, buffer_size, "%lus", secs);
    }
    
    return ESP_OK;
}

uint32_t utils_get_heap_usage_percent(void)
{
    size_t total = heap_caps_get_total_size(MALLOC_CAP_8BIT);
    size_t free = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    
    if (total == 0) return 0;
    
    uint32_t used = total - free;
    return (used * 100) / total;
}

uint32_t utils_get_psram_usage_percent(void)
{
    size_t total = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
    size_t free = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    
    if (total == 0) return 0;
    
    uint32_t used = total - free;
    return (used * 100) / total;
}