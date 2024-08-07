#include <Windows.h>
#include <detours.h>
#include <Locale_Simulator/Core/NlsPatcher.h>


auto wmain(int argc, wchar_t** argv) -> int
{
    if (argc == 1) { return 0; }

    ::PROCESS_INFORMATION pi{};
    ::STARTUPINFOW si{ .cb = sizeof(si) };

    const auto status = ::DetourCreateProcessWithDllExW(argv[1], nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi, "Locale_Simulator_Payload.dll", nullptr);
     if (!status) { return -1; }

    ZQF::LS::Core::NlsPatcher{ pi.hProcess, 0x3A4 }.Install();

    ::MessageBoxW(0, L"Click OK to resume", 0, 0);

    ::ResumeThread(pi.hThread);
    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);


    //PROCESS_INFORMATION pi = {0};
    //STARTUPINFOW si = {.cb = sizeof(si)};
    //WCHAR buffer[] = LR"(D:\Space\VN\Reverse\GSD\GSD3.0\SAKURA_006\gsd.exe)";
    //::CreateProcessW(nullptr, buffer, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi);

    //LEP::Utils::NlsPatcher::Install(pi.hProcess, LEP::Utils::NlsPatcher::SysVer::Nls_Windows11_x32, 0x3A4);

    //::MessageBoxA(0, 0, 0, 0);
    //::ResumeThread(pi.hThread);
    //::CloseHandle(pi.hProcess);
    //::CloseHandle(pi.hThread);
}
