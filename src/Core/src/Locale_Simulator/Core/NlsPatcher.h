#pragma once
#include <cstddef>
#include <windows.h>


namespace ZQF::LS::Core
{
	class NlsPatcher
	{
	public:
        static auto Install(const HANDLE hProcess) -> bool;

	public:
		static auto AfterWith() -> bool;
		static auto SaveInfoViaEnv(const std::size_t nHookVA, const std::size_t nShellCodeVA, const std::size_t nOldProtect) -> bool;

	};
} // namespace ZQF::LS::Core


