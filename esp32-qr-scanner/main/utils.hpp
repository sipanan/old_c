/**
 * @file utils.hpp
 * @brief Utility Functions Interface
 * @version 1.0.0
 * @date 2024
 * 
 * @copyright Copyright (c) 2024
 * @license MIT License
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "esp_err.h"
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

void utils_trigger_memory_recovery(void);
esp_err_t utils_format_uptime(uint32_t seconds, char *buffer, size_t buffer_size);
uint32_t utils_get_heap_usage_percent(void);
uint32_t utils_get_psram_usage_percent(void);

#ifdef __cplusplus
}
#endif

#endif // UTILS_HPP