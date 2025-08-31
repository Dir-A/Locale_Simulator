# Project
project("LocaleSimulator_Core" VERSION 1.0 LANGUAGES CXX)

# Library
add_library(${PROJECT_NAME}
    "src/Locale_Simulator/Core/NlsPatcher.cpp"
    "src/Locale_Simulator/Core/MsgPatcher.cpp"
    "src/Locale_Simulator/Core/WndPatcher.cpp"
    "src/Locale_Simulator/Core/GdiPatcher.cpp")
add_library(LocaleSimulator::Core ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC "src")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# External Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE LocaleSimulator::Utils)
