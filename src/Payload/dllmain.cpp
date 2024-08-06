#include <Windows.h>
#include <ZxHook/Hook.h>
#include <Locale_Simulator/Core/NlsPatcher.h>
#include <Locale_Simulator/Core/Win32uPatcher.h>


static auto StartHook(HMODULE /* hDllBase */) -> void
{
    ZQF::ZxHook::Detours::AfterWith();

    ZQF::LS::Core::NlsPatcher::AfterWith();

    ZQF::LS::Core::Win32uPatcher::Install();
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID /* lpReserved */)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ::StartHook(hModule);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" VOID __declspec(dllexport) LEP_EXPORT_FUNC() {}
