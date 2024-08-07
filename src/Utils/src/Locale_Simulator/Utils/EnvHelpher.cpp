#include "EnvHelpher.h"
#include <Windows.h>
#include <cstdio>


namespace ZQF::LS::Utils
{
    auto EnvHelper::LoadNum(const wchar_t* wpName) -> std::optional<std::size_t>
    {
#ifdef _WIN64
        LPCWSTR format = L"%ull";
#else
        LPCWSTR format = L"%ul";
#endif // _WIN64

        WCHAR buffer[64];
        const auto get_status = ::GetEnvironmentVariableW(wpName, buffer, 64);
        if (!get_status) { return std::nullopt; }

        std::size_t value{};
        ::swscanf_s(buffer, format, &value);
        return value ? std::optional{ value } : std::nullopt;
    }

    auto EnvHelper::StoreNum(const wchar_t* wpName, const std::size_t nValue) -> bool
    {
#ifdef _WIN64
        LPCWSTR format = L"%ull";
#else
        LPCWSTR format = L"%ul";
#endif // _WIN64

        WCHAR buffer[64];
        const auto format_status = ::wsprintfW(buffer, format, nValue);
        if (!format_status) { return false; }
        const auto set_status = ::SetEnvironmentVariableW(wpName, buffer);
        if (!set_status) { return false; }
        return true;
    }
}
