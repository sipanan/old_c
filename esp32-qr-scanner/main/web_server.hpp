/**
 * @file web_server.hpp
 * @brief Enterprise Web Server Interface
 * @version 1.0.0
 * @date 2024
 * 
 * @copyright Copyright (c) 2024
 * @license MIT License
 */

#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t web_server_init(void);
esp_err_t web_server_start(TaskHandle_t *task_handle);
esp_err_t web_server_stop(void);

#ifdef __cplusplus
}
#endif

#endif // WEB_SERVER_HPP