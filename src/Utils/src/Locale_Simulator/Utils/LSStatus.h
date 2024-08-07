#pragma once
#include <cstddef>
#include <optional>


namespace ZQF::LS::Utils
{
    class LSStatus
    {
    public:
        static auto GetCodePageNew() -> std::optional<std::size_t>;
        static auto GetCodePageRaw() -> std::optional<std::size_t>;
        static auto SetCodePage(const std::size_t nCodePageNew, const std::size_t nCodePageSys) -> bool;

        static auto GetCharset() -> std::optional<std::size_t>;
        static auto SetCharset(const std::size_t nCharset) -> bool;
    };
}
