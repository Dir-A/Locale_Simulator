#include <Windows.h>
#include <detours.h>
#include <Locale_Simulator/Core/NlsPatcher.h>
#include <Locale_Simulator/Utils/LSStatus.h>


auto wmain(int argc, wchar_t** argv) -> int
{
  if (argc == 1) { return 0; }

  ZQF::LS::Utils::LSStatus::SetCharset(0x80);
  ZQF::LS::Utils::LSStatus::SetCodePage(0x3A4, ::GetACP());

  ::PROCESS_INFORMATION pi{};
  ::STARTUPINFOW si{ .cb = sizeof(si) };

  const auto status = ::DetourCreateProcessWithDllExW(argv[1], nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi, "Locale_Simulator_Payload.dll", nullptr);
  if (!status) { return -1; }

  ZQF::LS::Core::NlsPatcher::Install(pi.hProcess);

  ::MessageBoxW(0, L"Click OK to resume", 0, 0);

  ::ResumeThread(pi.hThread);
  ::CloseHandle(pi.hProcess);
  ::CloseHandle(pi.hThread);
}
