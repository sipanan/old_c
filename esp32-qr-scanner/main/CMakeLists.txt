# Main component CMake file

idf_component_register(
    SRCS 
        "main.cpp"
        "camera_manager.cpp"
        "qr_detector.cpp"
        "web_server.cpp"
        "wifi_manager.cpp"
        "config_manager.cpp"
        "utils.cpp"
    INCLUDE_DIRS 
        "."
    REQUIRES 
        quirc
        nvs_flash
        esp_wifi
        esp_http_server
        esp_netif
        esp_event
        driver
        esp_timer
        freertos
        esp_system
        esp_common
        log
        spi_flash
        app_update
        esp_https_ota
        mbedtls
        spiffs
        vfs
)

# Set component properties
target_compile_options(${COMPONENT_LIB} PRIVATE 
    -Wno-unused-parameter
    -Wno-missing-field-initializers
)

# Add compile definitions
target_compile_definitions(${COMPONENT_LIB} PRIVATE
    CONFIG_CAMERA_MODEL_AI_THINKER=1
    CONFIG_ESP32_ENABLE_COREDUMP_TO_FLASH=1
)