# Project 
project("Locale_Simulator_Payload" VERSION 1.0 LANGUAGES CXX)

# Executable
add_library(${PROJECT_NAME} MODULE "dllmain.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# External Libraries
target_link_libraries("${PROJECT_NAME}" PRIVATE LocaleSimulator::Core Zut::ZxHook)
