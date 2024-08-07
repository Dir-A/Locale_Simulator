# LocaleSimulator
cmake_minimum_required(VERSION 3.25)

# Set Project
project(LocaleSimulator_Core)

# Set Library
add_library("${PROJECT_NAME}"
    "src/Locale_Simulator/Core/NlsPatcher.cpp"
    "src/Locale_Simulator/Core/MsgPatcher.cpp"
)
add_library(LocaleSimulator::Core ALIAS "${PROJECT_NAME}")
target_include_directories("${PROJECT_NAME}" PUBLIC "src")
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_23)
target_compile_options("${PROJECT_NAME}" PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Add Library
target_link_libraries("${PROJECT_NAME}" PRIVATE LocaleSimulator::Utils Zut::ZxHook)

# Warning
if(MSVC)
    target_compile_options("${PROJECT_NAME}" PRIVATE /W4)
else()
    target_compile_options("${PROJECT_NAME}" PRIVATE -Wall -Wextra)
endif()
