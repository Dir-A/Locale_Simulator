#include <print>
#include <iostream>
#include <Windows.h>



auto main() -> int
{
    try
    {

    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
