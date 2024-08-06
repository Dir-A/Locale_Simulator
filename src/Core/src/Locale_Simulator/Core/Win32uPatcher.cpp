#include <Locale_Simulator/Core/Win32uPatcher.h>
#include <Locale_Simulator/Utils/TextConvert.h>
#include <ZxHook/SHooker.h>
#include <Windows.h>


namespace ZQF::LS::Core
{
    static auto __stdcall NtUserMessageCall_Hook(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, ULONG_PTR xParam, DWORD xpfnProc, BOOL Ansi) -> LRESULT
    {
        if (!Ansi) { return ZxHook::SHooker<Core::NtUserMessageCall_Hook>::FnRaw(hWnd, Message, wParam, lParam, xParam, xpfnProc, Ansi); }

        // proc send ansi data
        {
            std::unique_ptr<wchar_t[]> buffer;

            switch (Message)
            {
            case WM_SETTEXT:
                buffer = Utils::MBCSToWide(reinterpret_cast<const char*>(lParam));
                lParam = reinterpret_cast<LPARAM>(buffer.get());
                Ansi = FALSE;
                break;
            }

            return ZxHook::SHooker<Core::NtUserMessageCall_Hook>::FnRaw(hWnd, Message, wParam, lParam, xParam, xpfnProc, Ansi);
        }

        // proc get ansi data
        {

        }
    }

    auto Win32uPatcher::Install() -> void
    {
        const auto win32u_handle = ::GetModuleHandleW(L"win32u.dll");
        if (win32u_handle == NULL) { return; }

        const auto fn_NtUserMessageCall_va = reinterpret_cast<std::size_t>(::GetProcAddress(win32u_handle, "NtUserMessageCall"));
        ZxHook::SHooker<Core::NtUserMessageCall_Hook>::Commit(fn_NtUserMessageCall_va);
    }
}
