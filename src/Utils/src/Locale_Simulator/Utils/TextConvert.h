#pragma once
#include <memory>
#include <string_view>


namespace ZQF::LS::Utils
{
    auto MBCSToWide(const std::string_view msPath) -> std::unique_ptr<wchar_t[]>;
} // namespace ZQF::LS::Utils
