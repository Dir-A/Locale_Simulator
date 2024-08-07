#pragma once
#include <cstddef>

namespace ZQF::LS::Core
{
    class LsStatus
    {
    public:
        static auto SetCodePage(const std::size_t nCodePage) -> bool;
    };
}
