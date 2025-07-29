# QuirC QR Code Library Component

idf_component_register(
    SRCS 
        "quirc_stub.c"
    INCLUDE_DIRS 
        "include"
    REQUIRES 
        esp_common
        log
)

target_compile_options(${COMPONENT_LIB} PRIVATE 
    -Wno-unused-parameter
)