#pragma once
#include <cstddef>
#include <optional>

// environment variable helper
namespace ZQF::LS::Utils
{
    class EnvHelper
    {
    public:
        static auto LoadNum(const wchar_t* wpName) -> std::optional<std::size_t>;
        static auto StoreNum(const wchar_t* wpName, const std::size_t nValue) -> bool;

    };

}
