#pragma once
#include <cstddef>
#include <memory>
#include <utility>
#include <string_view>



namespace ZQF::LS::Utils
{
    auto MBCSToWide(const std::string_view msPath) -> std::pair<std::unique_ptr<wchar_t[]>, std::size_t>;
} // namespace ZQF::LS::Utils
