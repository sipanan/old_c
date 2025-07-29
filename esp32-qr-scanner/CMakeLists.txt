# ESP32-CAM QR Scanner - Industrial Grade
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.16)

# Set the project name and description
project(esp32_qr_scanner VERSION 1.0.0 LANGUAGES C CXX)

# Include ESP-IDF CMake functions
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags for optimization and MISRA compliance
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -O2 -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O2 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti")

# Define the project
project(esp32_qr_scanner)