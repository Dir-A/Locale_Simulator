if(NOT MSVC)
  message(FATAL_ERROR "Detours Only MSVC")
endif()

include(FetchContent)
FetchContent_Declare(Detours URL https://github.com/microsoft/Detours/archive/refs/tags/v4.0.1.zip)
FetchContent_MakeAvailable(Detours)

if((detours_POPULATED) AND (NOT TARGET Detours_Static))
  add_library(Detours_Static STATIC
    "${detours_SOURCE_DIR}/src/creatwth.cpp"
    "${detours_SOURCE_DIR}/src/detours.cpp"
    "${detours_SOURCE_DIR}/src/disasm.cpp"
    "${detours_SOURCE_DIR}/src/disolarm64.cpp"
    "${detours_SOURCE_DIR}/src/disolx64.cpp"
    "${detours_SOURCE_DIR}/src/disolx86.cpp"
    "${detours_SOURCE_DIR}/src/image.cpp"
    "${detours_SOURCE_DIR}/src/modules.cpp"
  )
  add_library(Detours::Detours ALIAS Detours_Static)
  target_include_directories(Detours_Static INTERFACE "${detours_SOURCE_DIR}/src")
  target_compile_options(Detours_Static PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/nologo /WX /we4777 /we4800 /Zi /Gy /Gm- /Zl /DWIN32_LEAN_AND_MEAN>)
endif()