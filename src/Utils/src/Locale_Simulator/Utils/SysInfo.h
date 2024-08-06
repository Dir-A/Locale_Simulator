#pragma once
#include <Windows.h>
#include <cstddef>


namespace ZQF::LS::Utils
{
	class SysInfo
	{
	public:
		static auto GetBuildNumber() -> std::size_t;
		static auto IsProcessx64(const HANDLE hProcess) -> bool;
	};
} // namespace ZQF::LS::Utils

