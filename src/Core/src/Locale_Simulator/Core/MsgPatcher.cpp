#include "MsgPatcher.h"
#include <Windows.h>
#include <Locale_Simulator/Utils/SHooker.h>
#include <Locale_Simulator/Utils/TextConvert.h>


namespace ZQF::LS::Core
{
  static auto __stdcall NtUserMessageCall_Hook(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, ULONG_PTR xParam, DWORD xpfnProc, BOOL Ansi) -> LRESULT
  {
    if (!Ansi) { return Utils::SHooker<Core::NtUserMessageCall_Hook>::FnRaw(hWnd, Message, wParam, lParam, xParam, xpfnProc, Ansi); }

    // proc send ansi data
    {
      std::pair<std::unique_ptr<wchar_t[]>, std::size_t> buffer;

      switch (Message)
      {
      case WM_SETTEXT:
        buffer = Utils::MBCSToWide(reinterpret_cast<const char*>(lParam));
        lParam = reinterpret_cast<LPARAM>(buffer.first.get());
        Ansi = FALSE;
        break;
      }

      return Utils::SHooker<Core::NtUserMessageCall_Hook>::FnRaw(hWnd, Message, wParam, lParam, xParam, xpfnProc, Ansi);
    }

    // proc get ansi data
    {

    }
  }

  static auto FindNtUserMessageCall() -> std::size_t
  {
    if (const auto win32u_handle = ::GetModuleHandleW(L"win32u.dll"); win32u_handle != NULL)
    {
      return reinterpret_cast<std::size_t>(::GetProcAddress(win32u_handle, "NtUserMessageCall"));
    }

    if (const auto user32_handle = ::GetModuleHandleW(L"user32.dll"); user32_handle != NULL)
    {
      const auto gapfnScSendMessage = ::GetProcAddress(user32_handle, "gapfnScSendMessage");
      if (gapfnScSendMessage == NULL) { return 0; }
      return reinterpret_cast<std::size_t*>(gapfnScSendMessage)[0];
    }

    return 0;
  }

  auto MsgPatcher::Install() -> bool
  {
    const auto fn_NtUserMessageCall_va = Core::FindNtUserMessageCall();
    if (fn_NtUserMessageCall_va == 0) { return false; }
    Utils::SHooker<Core::NtUserMessageCall_Hook>::AttachAndCommit(fn_NtUserMessageCall_va);
    return true;
  }
}
