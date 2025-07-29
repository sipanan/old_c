/**
 * @file qr_detector.hpp
 * @brief Enterprise-Grade QR Code Detection Interface
 * @version 1.0.0
 * @date 2024
 * 
 * High-performance QR code detection using QuirC library with advanced
 * image processing, error correction, and enterprise-grade reliability.
 * Target: 99.5% accuracy for standard QR codes, >95% for damaged codes.
 * 
 * @copyright Copyright (c) 2024
 * @license MIT License
 */

#ifndef QR_DETECTOR_HPP
#define QR_DETECTOR_HPP

#include <cstdint>
#include <cstddef>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "camera_manager.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief QR code data types
 */
typedef enum {
    QR_DATA_TYPE_UNKNOWN = 0,
    QR_DATA_TYPE_NUMERIC,
    QR_DATA_TYPE_ALPHANUMERIC,
    QR_DATA_TYPE_BINARY,
    QR_DATA_TYPE_KANJI,
    QR_DATA_TYPE_ECI,
    QR_DATA_TYPE_STRUCTURED_APPEND,
    QR_DATA_TYPE_FNCI_FIRST,
    QR_DATA_TYPE_FNCI_SECOND
} qr_data_type_t;

/**
 * @brief QR code error correction levels
 */
typedef enum {
    QR_ECC_LEVEL_L = 0,  ///< Low (~7% error correction)
    QR_ECC_LEVEL_M,      ///< Medium (~15% error correction)
    QR_ECC_LEVEL_Q,      ///< Quartile (~25% error correction)
    QR_ECC_LEVEL_H       ///< High (~30% error correction)
} qr_ecc_level_t;

/**
 * @brief QR code versions (size indicators)
 */
typedef enum {
    QR_VERSION_1 = 1,    ///< 21x21 modules
    QR_VERSION_2,        ///< 25x25 modules
    QR_VERSION_3,        ///< 29x29 modules
    QR_VERSION_4,        ///< 33x33 modules
    QR_VERSION_5,        ///< 37x37 modules
    QR_VERSION_6,        ///< 41x41 modules
    QR_VERSION_7,        ///< 45x45 modules
    QR_VERSION_8,        ///< 49x49 modules
    QR_VERSION_9,        ///< 53x53 modules
    QR_VERSION_10,       ///< 57x57 modules
    // ... up to 40 (177x177 modules)
    QR_VERSION_MAX = 40
} qr_version_t;

/**
 * @brief Special QR content types for automatic parsing
 */
typedef enum {
    QR_CONTENT_TYPE_UNKNOWN = 0,
    QR_CONTENT_TYPE_URL,
    QR_CONTENT_TYPE_WIFI,
    QR_CONTENT_TYPE_VCARD,
    QR_CONTENT_TYPE_SMS,
    QR_CONTENT_TYPE_EMAIL,
    QR_CONTENT_TYPE_PHONE,
    QR_CONTENT_TYPE_GEO,
    QR_CONTENT_TYPE_PLAIN_TEXT
} qr_content_type_t;

/**
 * @brief WiFi QR code information
 */
typedef struct {
    char ssid[64];           ///< WiFi SSID
    char password[64];       ///< WiFi password
    char security[16];       ///< Security type (WPA, WEP, nopass)
    bool hidden;             ///< Hidden network flag
} qr_wifi_info_t;

/**
 * @brief VCard contact information
 */
typedef struct {
    char name[128];          ///< Full name
    char phone[32];          ///< Phone number
    char email[64];          ///< Email address
    char organization[64];   ///< Organization
    char title[64];          ///< Job title
    char url[128];           ///< Website URL
} qr_vcard_info_t;

/**
 * @brief Geographic coordinates
 */
typedef struct {
    double latitude;         ///< Latitude in decimal degrees
    double longitude;        ///< Longitude in decimal degrees
    double altitude;         ///< Altitude in meters (optional)
} qr_geo_info_t;

/**
 * @brief QR code detection result
 */
typedef struct {
    // Basic QR information
    qr_version_t version;             ///< QR code version (1-40)
    qr_ecc_level_t ecc_level;        ///< Error correction level
    int mask_pattern;                 ///< Mask pattern used
    qr_data_type_t data_type;        ///< Data type mode
    
    // Content information
    char *payload;                    ///< Raw payload data
    size_t payload_length;            ///< Payload length in bytes
    qr_content_type_t content_type;   ///< Detected content type
    
    // Parsed content (based on content_type)
    union {
        qr_wifi_info_t wifi;          ///< WiFi information
        qr_vcard_info_t vcard;        ///< VCard contact info
        qr_geo_info_t geo;            ///< Geographic coordinates
        char url[256];                ///< URL string
        char plain_text[512];         ///< Plain text content
    } parsed_content;
    
    // Detection metadata
    uint64_t detection_time_us;       ///< Detection timestamp
    uint32_t processing_time_ms;      ///< Processing time in milliseconds
    float confidence_score;           ///< Detection confidence (0.0-1.0)
    uint32_t frame_sequence;          ///< Source frame sequence number
    
    // Geometric information
    struct {
        int x, y;                     ///< Corner coordinates
    } corners[4];                     ///< QR code corners in image
    
    int center_x, center_y;           ///< QR code center
    float rotation_angle;             ///< Rotation angle in degrees
    float scale_factor;               ///< Scale relative to standard size
    
    // Quality metrics
    float edge_sharpness;             ///< Edge sharpness score
    float contrast_ratio;             ///< Contrast ratio
    uint8_t error_correction_used;    ///< Percentage of error correction used
    bool is_partial_decode;           ///< True if partially decoded
    
} qr_detection_result_t;

/**
 * @brief QR detector statistics for monitoring
 */
typedef struct {
    uint32_t total_frames_processed;  ///< Total frames analyzed
    uint32_t qr_codes_detected;       ///< Total QR codes detected
    uint32_t detection_failures;      ///< Failed detection attempts
    uint32_t partial_decodes;         ///< Partially decoded QR codes
    uint32_t avg_processing_time_ms;  ///< Average processing time
    uint32_t max_processing_time_ms;  ///< Maximum processing time
    float success_rate;               ///< Detection success rate (0.0-1.0)
    uint32_t memory_usage_bytes;      ///< Current memory usage
    uint64_t last_detection_time;     ///< Last successful detection timestamp
    
    // Version and error correction statistics
    uint32_t version_stats[QR_VERSION_MAX]; ///< Count by version
    uint32_t ecc_stats[4];            ///< Count by ECC level
    uint32_t content_type_stats[9];   ///< Count by content type
    
} qr_detector_stats_t;

/**
 * @brief QR detection configuration
 */
typedef struct {
    // Detection sensitivity
    float detection_threshold;        ///< Detection threshold (0.1-0.9)
    uint8_t max_qr_codes_per_frame;  ///< Maximum QR codes to detect per frame
    bool enable_rotation_detection;   ///< Enable rotation detection
    bool enable_perspective_correction; ///< Enable perspective correction
    
    // Image preprocessing
    bool enable_histogram_equalization; ///< Improve contrast
    bool enable_noise_reduction;      ///< Reduce image noise
    bool enable_edge_enhancement;     ///< Enhance edges
    uint8_t brightness_adjustment;    ///< Brightness adjustment (-50 to 50)
    uint8_t contrast_adjustment;      ///< Contrast adjustment (-50 to 50)
    
    // Performance settings
    bool use_multi_threading;         ///< Use multiple cores for processing
    uint8_t processing_priority;      ///< Task priority (1-10)
    uint32_t timeout_ms;              ///< Processing timeout per frame
    bool enable_result_caching;       ///< Cache recent results
    
    // Quality control
    float min_confidence_threshold;   ///< Minimum confidence for valid detection
    bool require_checksum_validation; ///< Require data checksum validation
    bool enable_error_recovery;       ///< Attempt error recovery for damaged codes
    
} qr_detector_config_t;

/**
 * @brief QR detection callback function
 * @param result Detection result (nullptr if no QR code found)
 * @param user_data User-provided callback data
 * @return ESP_OK to continue processing, other values to stop
 */
typedef esp_err_t (*qr_detection_callback_t)(const qr_detection_result_t *result, void *user_data);

/**
 * @brief Initialize QR detector with enterprise-grade configuration
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_init(void);

/**
 * @brief Deinitialize QR detector and free resources
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_deinit(void);

/**
 * @brief Start QR detection task
 * @param task_handle Pointer to store task handle
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_start_task(TaskHandle_t *task_handle);

/**
 * @brief Stop QR detection task
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_stop_task(void);

/**
 * @brief Process a single frame for QR detection (blocking)
 * @param frame Input camera frame
 * @param results Array to store detection results
 * @param max_results Maximum number of results to return
 * @param actual_results Number of QR codes actually detected
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_process_frame(const camera_frame_t *frame,
                                   qr_detection_result_t *results,
                                   uint8_t max_results,
                                   uint8_t *actual_results);

/**
 * @brief Register callback for QR detection events
 * @param callback Callback function
 * @param user_data User data to pass to callback
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_register_callback(qr_detection_callback_t callback, void *user_data);

/**
 * @brief Unregister QR detection callback
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_unregister_callback(void);

/**
 * @brief Update QR detector configuration
 * @param config New configuration
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_update_config(const qr_detector_config_t *config);

/**
 * @brief Get current QR detector configuration
 * @param config Pointer to store current configuration
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_get_config(qr_detector_config_t *config);

/**
 * @brief Get QR detector statistics
 * @param stats Pointer to store statistics
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_get_stats(qr_detector_stats_t *stats);

/**
 * @brief Reset QR detector statistics
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_reset_stats(void);

/**
 * @brief Parse QR code content based on type
 * @param result Detection result to parse
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_parse_content(qr_detection_result_t *result);

/**
 * @brief Validate QR code data integrity
 * @param result Detection result to validate
 * @return ESP_OK if valid, error code if invalid
 */
esp_err_t qr_detector_validate_data(const qr_detection_result_t *result);

/**
 * @brief Get supported QR code versions
 * @param versions Array to store supported versions
 * @param max_versions Maximum number of versions to return
 * @param actual_versions Number of versions actually supported
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_get_supported_versions(qr_version_t *versions,
                                           uint8_t max_versions,
                                           uint8_t *actual_versions);

/**
 * @brief Perform QR detector self-test
 * @return ESP_OK if test passed, error code on failure
 */
esp_err_t qr_detector_self_test(void);

/**
 * @brief Free QR detection result memory
 * @param result Result to free
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_free_result(qr_detection_result_t *result);

/**
 * @brief Get memory usage information
 * @param heap_used Heap memory used in bytes
 * @param psram_used PSRAM used in bytes
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_get_memory_usage(size_t *heap_used, size_t *psram_used);

/**
 * @brief Enable/disable debug mode for QR detection
 * @param enable True to enable debug mode
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_set_debug_mode(bool enable);

/**
 * @brief Get last detection error information
 * @param error_code Pointer to store error code
 * @param error_message Buffer to store error message
 * @param message_size Size of error message buffer
 * @return ESP_OK on success, error code on failure
 */
esp_err_t qr_detector_get_last_error(esp_err_t *error_code,
                                    char *error_message,
                                    size_t message_size);

#ifdef __cplusplus
}
#endif

#endif // QR_DETECTOR_HPP