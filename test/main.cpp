#include <print>
#include <iostream>



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
