/**
 * @file camera_manager.hpp
 * @brief ESP32-CAM Camera Management Interface
 * @version 1.0.0
 * @date 2024
 * 
 * High-performance camera interface for ESP32-CAM with enterprise-grade
 * reliability, PSRAM optimization, and advanced image processing features.
 * 
 * @copyright Copyright (c) 2024
 * @license MIT License
 */

#ifndef CAMERA_MANAGER_HPP
#define CAMERA_MANAGER_HPP

#include <cstdint>
#include <cstddef>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_camera.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Camera frame buffer structure optimized for PSRAM usage
 */
typedef struct {
    uint8_t *buffer;          ///< Image data buffer (allocated in PSRAM)
    size_t length;            ///< Buffer length in bytes
    uint32_t width;           ///< Image width in pixels
    uint32_t height;          ///< Image height in pixels
    pixformat_t format;       ///< Pixel format
    uint64_t timestamp_us;    ///< Capture timestamp in microseconds
    uint32_t sequence_number; ///< Frame sequence number
    float exposure_time_ms;   ///< Actual exposure time
    uint16_t gain;            ///< Applied gain value
    bool is_valid;            ///< Frame validity flag
} camera_frame_t;

/**
 * @brief Camera statistics for monitoring and optimization
 */
typedef struct {
    uint32_t frames_captured;     ///< Total frames captured
    uint32_t frames_dropped;      ///< Frames dropped due to processing delay
    uint32_t capture_errors;      ///< Number of capture errors
    uint32_t avg_capture_time_ms; ///< Average capture time in milliseconds
    uint32_t max_capture_time_ms; ///< Maximum capture time in milliseconds
    uint32_t psram_usage_bytes;   ///< Current PSRAM usage for camera buffers
    float fps_actual;             ///< Actual FPS achieved
    float fps_target;             ///< Target FPS setting
    uint32_t last_frame_timestamp; ///< Last successful frame timestamp
} camera_stats_t;

/**
 * @brief Camera configuration with enterprise-grade settings
 */
typedef struct {
    framesize_t frame_size;       ///< Frame size (FRAMESIZE_VGA recommended for QR)
    pixformat_t pixel_format;     ///< Pixel format (PIXFORMAT_GRAYSCALE for QR)
    uint8_t jpeg_quality;         ///< JPEG quality (1-63, lower = higher quality)
    uint8_t frame_buffer_count;   ///< Number of frame buffers (2-4 recommended)
    bool enable_psram;            ///< Use PSRAM for frame buffers
    uint8_t grab_mode;            ///< Frame grab mode
    
    // Image quality settings
    int8_t brightness;            ///< Brightness (-2 to 2)
    int8_t contrast;              ///< Contrast (-2 to 2)
    int8_t saturation;            ///< Saturation (-2 to 2)
    int8_t special_effect;        ///< Special effects
    int8_t wb_mode;               ///< White balance mode
    bool awb_gain;                ///< Auto white balance gain
    uint8_t agc_gain;             ///< AGC gain
    bool gain_ctrl;               ///< AGC enable
    uint8_t exposure_ctrl;        ///< AEC sensor
    bool hmirror;                 ///< Horizontal mirror
    bool vflip;                   ///< Vertical flip
    
    // Advanced settings for QR optimization
    bool auto_exposure;           ///< Auto exposure enable
    uint16_t manual_exposure;     ///< Manual exposure value
    bool auto_gain;               ///< Auto gain enable
    uint8_t manual_gain;          ///< Manual gain value
    uint8_t sharpness;            ///< Image sharpness
    uint8_t denoise;              ///< Noise reduction level
} camera_config_enterprise_t;

/**
 * @brief Camera callback for frame processing
 * @param frame Captured frame data
 * @param user_data User-provided callback data
 * @return ESP_OK on success, error code on failure
 */
typedef esp_err_t (*camera_frame_callback_t)(const camera_frame_t *frame, void *user_data);

/**
 * @brief Initialize camera manager with enterprise-grade configuration
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_init(void);

/**
 * @brief Deinitialize camera manager and free resources
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_deinit(void);

/**
 * @brief Start camera capture task
 * @param task_handle Pointer to store task handle
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_start_capture_task(TaskHandle_t *task_handle);

/**
 * @brief Stop camera capture task
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_stop_capture_task(void);

/**
 * @brief Capture a single frame (blocking)
 * @param frame Pointer to store captured frame
 * @param timeout_ms Timeout in milliseconds
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_capture_frame(camera_frame_t *frame, uint32_t timeout_ms);

/**
 * @brief Release a captured frame and return buffer to pool
 * @param frame Frame to release
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_release_frame(camera_frame_t *frame);

/**
 * @brief Register callback for frame processing
 * @param callback Callback function
 * @param user_data User data to pass to callback
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_register_frame_callback(camera_frame_callback_t callback, void *user_data);

/**
 * @brief Unregister frame callback
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_unregister_frame_callback(void);

/**
 * @brief Get camera statistics
 * @param stats Pointer to store statistics
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_get_stats(camera_stats_t *stats);

/**
 * @brief Reset camera statistics
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_reset_stats(void);

/**
 * @brief Update camera configuration dynamically
 * @param config New camera configuration
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_update_config(const camera_config_enterprise_t *config);

/**
 * @brief Get current camera configuration
 * @param config Pointer to store current configuration
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_get_config(camera_config_enterprise_t *config);

/**
 * @brief Enable/disable auto-focus (if supported)
 * @param enable True to enable auto-focus
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_set_autofocus(bool enable);

/**
 * @brief Set manual focus distance (if supported)
 * @param distance Focus distance (0-1023)
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_set_focus_distance(uint16_t distance);

/**
 * @brief Trigger auto-exposure and auto-white balance calibration
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_calibrate_auto_settings(void);

/**
 * @brief Get frame from queue (non-blocking)
 * @param frame Pointer to store frame
 * @return ESP_OK on success, ESP_ERR_NOT_FOUND if no frame available
 */
esp_err_t camera_manager_get_frame_nonblocking(camera_frame_t *frame);

/**
 * @brief Check if camera is ready for capture
 * @return true if ready, false otherwise
 */
bool camera_manager_is_ready(void);

/**
 * @brief Get current frame rate
 * @return Current FPS
 */
float camera_manager_get_fps(void);

/**
 * @brief Set target frame rate
 * @param fps Target FPS (1-60)
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_set_fps(float fps);

/**
 * @brief Optimize camera settings for QR code detection
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_optimize_for_qr(void);

/**
 * @brief Get memory usage information
 * @param psram_used PSRAM usage in bytes
 * @param sram_used SRAM usage in bytes
 * @return ESP_OK on success, error code on failure
 */
esp_err_t camera_manager_get_memory_usage(size_t *psram_used, size_t *sram_used);

/**
 * @brief Perform camera self-test
 * @return ESP_OK if test passed, error code on failure
 */
esp_err_t camera_manager_self_test(void);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_MANAGER_HPP