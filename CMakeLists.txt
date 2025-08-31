# Locale Simulator Main
cmake_minimum_required(VERSION 3.25)

# Project Name
project("Locale_Simulator_Main" VERSION 1.0 LANGUAGES CXX)

# Config
include("cmake/ZQFSettings.cmake")

# Source
add_subdirectory("third/Detours/")
add_subdirectory("src")

# On Test Config
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory("tryout")
endif()
