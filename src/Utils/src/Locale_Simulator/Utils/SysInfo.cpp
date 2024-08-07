#include <Locale_Simulator/Utils/SysInfo.h>
#include <cstdio>


namespace ZQF::LS::Utils
{
	auto SysInfo::GetBuildNumber() -> std::size_t
	{
		HKEY hkey{};
		const auto status = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", NULL, KEY_READ, &hkey);
		if (status == ERROR_SUCCESS)
		{
			DWORD type{};
			BYTE buffer[64];
			DWORD bytes = 62;
			::RegQueryValueExW(hkey, L"CurrentBuildNumber", nullptr, &type, buffer, &bytes);
			::RegCloseKey(hkey);

			if (type != REG_SZ) { return 0; }

			buffer[bytes + 0] = {};
			buffer[bytes + 1] = {};

			DWORD build_number{};
			::swscanf_s((LPCWSTR)buffer, L"%d", &build_number);
			return static_cast<std::size_t>(build_number);
		}

		return 0;
	}

	auto SysInfo::IsProcessI386(const HANDLE hProcess) -> bool
	{
        const auto kernel32_handle = ::GetModuleHandleW(L"kernel32.dll");
        if (kernel32_handle == NULL) { return false; }

        const auto fn_IsWow64Process2 = ::GetProcAddress(kernel32_handle, "IsWow64Process2");
        if (fn_IsWow64Process2 != NULL)
        {
            USHORT process_machine;
            const auto status = ((BOOL(WINAPI*)(HANDLE, USHORT*, USHORT*))(fn_IsWow64Process2))(hProcess, &process_machine, nullptr);
            if (!status) { return false; }
            return (process_machine == IMAGE_FILE_MACHINE_I386);
        }
        else
        {
            const auto fn_IsWow64Process = ::GetProcAddress(kernel32_handle, "IsWow64Process");
            if (fn_IsWow64Process == NULL) { return false; }
            BOOL is_wow64 = FALSE;
            const auto status = ((BOOL(WINAPI*)(HANDLE, PBOOL))(fn_IsWow64Process))(hProcess, &is_wow64);
            if (!status) { return false; }
            return is_wow64 != FALSE;
        }
	}
} // namespace ZQF::LS::Utils
