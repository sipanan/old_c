/**
 * @file web_server.cpp
 * @brief Enterprise Web Server Implementation
 * @version 1.0.0
 * @date 2024
 * 
 * @copyright Copyright (c) 2024
 * @license MIT License
 */

#include "web_server.hpp"
#include "esp_http_server.h"
#include "esp_log.h"
#include "camera_manager.hpp"
#include "qr_detector.hpp"
#include "wifi_manager.hpp"
#include <cstring>

static const char *TAG = "WEB_SERVER";

static httpd_handle_t server = nullptr;
static bool server_running = false;

// HTML content for the main page
static const char* main_page_html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32-CAM QR Scanner</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; background: #f0f0f0; }
        .container { max-width: 800px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; }
        .header { text-align: center; color: #333; }
        .stats { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 15px; margin: 20px 0; }
        .stat-card { background: #007bff; color: white; padding: 15px; border-radius: 8px; text-align: center; }
        .stat-value { font-size: 24px; font-weight: bold; }
        .stat-label { font-size: 14px; opacity: 0.9; }
        .qr-result { background: #e8f5e8; border: 1px solid #28a745; padding: 15px; margin: 10px 0; border-radius: 5px; }
        .camera-view { text-align: center; margin: 20px 0; }
        .btn { background: #007bff; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; margin: 5px; }
        .btn:hover { background: #0056b3; }
        .status { padding: 10px; margin: 10px 0; border-radius: 5px; }
        .status.connected { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
        .status.disconnected { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🔍 ESP32-CAM QR Scanner</h1>
            <p>Industrial Grade QR Code Detection System</p>
        </div>
        
        <div id="wifi-status" class="status"></div>
        
        <div class="stats">
            <div class="stat-card">
                <div class="stat-value" id="uptime">0s</div>
                <div class="stat-label">System Uptime</div>
            </div>
            <div class="stat-card">
                <div class="stat-value" id="qr-count">0</div>
                <div class="stat-label">QR Codes Detected</div>
            </div>
            <div class="stat-card">
                <div class="stat-value" id="fps">0.0</div>
                <div class="stat-label">Camera FPS</div>
            </div>
            <div class="stat-card">
                <div class="stat-value" id="success-rate">0%</div>
                <div class="stat-label">Detection Rate</div>
            </div>
        </div>
        
        <div class="camera-view">
            <h3>Camera Feed</h3>
            <img id="camera-stream" src="/stream" alt="Camera Stream" style="max-width: 100%; border: 2px solid #ddd; border-radius: 8px;">
            <br>
            <button class="btn" onclick="optimizeCamera()">Optimize for QR</button>
            <button class="btn" onclick="resetStats()">Reset Statistics</button>
        </div>
        
        <div id="qr-results">
            <h3>Recent QR Detections</h3>
            <div id="results-list"></div>
        </div>
    </div>

    <script>
        function updateStats() {
            fetch('/api/stats')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('uptime').textContent = data.uptime + 's';
                    document.getElementById('qr-count').textContent = data.qr_count;
                    document.getElementById('fps').textContent = data.fps.toFixed(1);
                    document.getElementById('success-rate').textContent = (data.success_rate * 100).toFixed(1) + '%';
                    
                    const status = document.getElementById('wifi-status');
                    if (data.wifi_connected) {
                        status.className = 'status connected';
                        status.innerHTML = '✅ WiFi Connected - IP: ' + data.ip_address;
                    } else {
                        status.className = 'status disconnected';
                        status.innerHTML = '❌ WiFi Disconnected';
                    }
                })
                .catch(err => console.error('Failed to fetch stats:', err));
        }
        
        function optimizeCamera() {
            fetch('/api/optimize', { method: 'POST' })
                .then(response => response.text())
                .then(data => alert('Camera optimized for QR detection'))
                .catch(err => alert('Failed to optimize camera'));
        }
        
        function resetStats() {
            fetch('/api/reset-stats', { method: 'POST' })
                .then(response => response.text())
                .then(data => alert('Statistics reset'))
                .catch(err => alert('Failed to reset statistics'));
        }
        
        function updateQRResults() {
            fetch('/api/recent-qr')
                .then(response => response.json())
                .then(data => {
                    const resultsDiv = document.getElementById('results-list');
                    resultsDiv.innerHTML = '';
                    
                    data.forEach(result => {
                        const div = document.createElement('div');
                        div.className = 'qr-result';
                        div.innerHTML = `
                            <strong>Type:</strong> ${result.type}<br>
                            <strong>Content:</strong> ${result.content}<br>
                            <strong>Confidence:</strong> ${(result.confidence * 100).toFixed(1)}%<br>
                            <strong>Time:</strong> ${new Date(result.timestamp).toLocaleString()}
                        `;
                        resultsDiv.appendChild(div);
                    });
                })
                .catch(err => console.error('Failed to fetch QR results:', err));
        }
        
        // Update stats and QR results every 2 seconds
        setInterval(updateStats, 2000);
        setInterval(updateQRResults, 5000);
        
        // Initial load
        updateStats();
        updateQRResults();
    </script>
</body>
</html>
)";

static esp_err_t main_page_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, main_page_html, strlen(main_page_html));
}

static esp_err_t stats_handler(httpd_req_t *req)
{
    char response[512];
    
    // Get system stats
    uint32_t uptime = 0; // app_get_uptime_seconds();
    uint32_t qr_count = 0; // app_get_qr_codes_processed();
    float fps = 0.0f; // camera_manager_get_fps();
    
    // Get QR detector stats
    qr_detector_stats_t qr_stats;
    esp_err_t ret = qr_detector_get_stats(&qr_stats);
    float success_rate = (ret == ESP_OK) ? qr_stats.success_rate : 0.0f;
    
    // Get WiFi status
    wifi_state_t wifi_state = wifi_manager_get_state();
    bool wifi_connected = (wifi_state == WIFI_STATE_CONNECTED);
    
    char ip_str[16] = "0.0.0.0";
    if (wifi_connected) {
        wifi_manager_get_ip_info(ip_str, sizeof(ip_str));
    }
    
    snprintf(response, sizeof(response),
        "{"
        "\"uptime\":%lu,"
        "\"qr_count\":%lu,"
        "\"fps\":%.2f,"
        "\"success_rate\":%.3f,"
        "\"wifi_connected\":%s,"
        "\"ip_address\":\"%s\""
        "}",
        uptime, qr_count, fps, success_rate,
        wifi_connected ? "true" : "false", ip_str);
    
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, response, strlen(response));
}

static esp_err_t optimize_handler(httpd_req_t *req)
{
    esp_err_t ret = camera_manager_optimize_for_qr();
    
    if (ret == ESP_OK) {
        httpd_resp_send(req, "OK", 2);
    } else {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to optimize camera");
    }
    
    return ESP_OK;
}

static esp_err_t reset_stats_handler(httpd_req_t *req)
{
    esp_err_t ret1 = camera_manager_reset_stats();
    esp_err_t ret2 = qr_detector_reset_stats();
    
    if (ret1 == ESP_OK && ret2 == ESP_OK) {
        httpd_resp_send(req, "OK", 2);
    } else {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to reset statistics");
    }
    
    return ESP_OK;
}

static esp_err_t recent_qr_handler(httpd_req_t *req)
{
    // For now, return empty array - would be populated with real recent QR results
    const char *response = "[]";
    
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, response, strlen(response));
}

static esp_err_t stream_handler(httpd_req_t *req)
{
    // Simplified camera stream - would implement MJPEG streaming in real version
    const char *response = "Camera stream not implemented in demo";
    httpd_resp_send_err(req, HTTPD_501_NOT_IMPLEMENTED, response);
    return ESP_OK;
}

esp_err_t web_server_init(void)
{
    ESP_LOGI(TAG, "Initializing web server");
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    config.max_open_sockets = 7;
    config.stack_size = 8192;
    
    esp_err_t ret = httpd_start(&server, &config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start HTTP server: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Register URI handlers
    httpd_uri_t main_page_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = main_page_handler,
        .user_ctx = nullptr
    };
    httpd_register_uri_handler(server, &main_page_uri);
    
    httpd_uri_t stats_uri = {
        .uri = "/api/stats",
        .method = HTTP_GET,
        .handler = stats_handler,
        .user_ctx = nullptr
    };
    httpd_register_uri_handler(server, &stats_uri);
    
    httpd_uri_t optimize_uri = {
        .uri = "/api/optimize",
        .method = HTTP_POST,
        .handler = optimize_handler,
        .user_ctx = nullptr
    };
    httpd_register_uri_handler(server, &optimize_uri);
    
    httpd_uri_t reset_stats_uri = {
        .uri = "/api/reset-stats",
        .method = HTTP_POST,
        .handler = reset_stats_handler,
        .user_ctx = nullptr
    };
    httpd_register_uri_handler(server, &reset_stats_uri);
    
    httpd_uri_t recent_qr_uri = {
        .uri = "/api/recent-qr",
        .method = HTTP_GET,
        .handler = recent_qr_handler,
        .user_ctx = nullptr
    };
    httpd_register_uri_handler(server, &recent_qr_uri);
    
    httpd_uri_t stream_uri = {
        .uri = "/stream",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = nullptr
    };
    httpd_register_uri_handler(server, &stream_uri);
    
    server_running = true;
    ESP_LOGI(TAG, "Web server initialized and started on port 80");
    
    return ESP_OK;
}

esp_err_t web_server_start(TaskHandle_t *task_handle)
{
    if (server_running) {
        ESP_LOGI(TAG, "Web server already running");
        if (task_handle) {
            *task_handle = nullptr; // No dedicated task for HTTP server
        }
        return ESP_OK;
    }
    
    return web_server_init();
}

esp_err_t web_server_stop(void)
{
    if (server && server_running) {
        esp_err_t ret = httpd_stop(server);
        if (ret == ESP_OK) {
            server = nullptr;
            server_running = false;
            ESP_LOGI(TAG, "Web server stopped");
        }
        return ret;
    }
    
    return ESP_OK;
}