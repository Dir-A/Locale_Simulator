# Project
project(LocaleSimulator_Utils)

# Set Library
add_library(${PROJECT_NAME}
    "src/Locale_Simulator/Utils/SysInfo.cpp"
    "src/Locale_Simulator/Utils/TextConvert.cpp"
    "src/Locale_Simulator/Utils/VAHelper.cpp"
    "src/Locale_Simulator/Utils/EnvHelpher.cpp"
    "src/Locale_Simulator/Utils/LSStatus.cpp"
    "src/Locale_Simulator/Utils/SHooker.cpp")
add_library(LocaleSimulator::Utils ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC "src")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# External Libraries
target_link_libraries("${PROJECT_NAME}" PRIVATE LocaleSimulator::Core Detours::Detours)
