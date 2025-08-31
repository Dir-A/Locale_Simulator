#include "WndPatcher.h"
#include <Windows.h>
#include <Locale_Simulator/Utils/SHooker.h>
#include <Locale_Simulator/Utils/TextConvert.h>


namespace ZQF::LS::Core
{
  typedef struct _LARGE_STRING
  {
    ULONG Length;
    ULONG MaximumLength : 31;
    ULONG bAnsi : 1;
    PVOID Buffer;
  } LARGE_STRING, * PLARGE_STRING;


  static auto __stdcall NtUserCreateWindowEx_Hook(DWORD dwExStyle, PLARGE_STRING plstrClassName, PLARGE_STRING plstrClsVersion, PLARGE_STRING plstrWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam, DWORD dwFlags, PVOID acbiBuffer) -> HWND
  {
    std::pair<std::unique_ptr<wchar_t[]>, std::size_t> buffer_0;
    std::pair<std::unique_ptr<wchar_t[]>, std::size_t> buffer_1;
    std::pair<std::unique_ptr<wchar_t[]>, std::size_t> buffer_2;

    if (dwExStyle & 0x80000000)
    {
      if (reinterpret_cast<SIZE_T>(plstrClassName) & 0xFFFF0000)
      {
        if (plstrClassName->bAnsi)
        {
          buffer_0 = Utils::MBCSToWide({ reinterpret_cast<const char*>(plstrClassName->Buffer), plstrClassName->Length });
          plstrClassName->Buffer = buffer_0.first.get();
          plstrClassName->Length = static_cast<ULONG>(buffer_0.second * sizeof(wchar_t));
          plstrClassName->MaximumLength = plstrClassName->Length + (1 * sizeof(wchar_t));
          plstrClassName->bAnsi = FALSE;
        }
      }

      if (reinterpret_cast<SIZE_T>(plstrClsVersion) & 0xFFFF0000)
      {

        if (plstrClsVersion->bAnsi)
        {
          buffer_1 = Utils::MBCSToWide({ reinterpret_cast<const char*>(plstrClsVersion->Buffer), plstrClsVersion->Length });
          plstrClsVersion->Buffer = buffer_1.first.get();
          plstrClsVersion->Length = static_cast<ULONG>(buffer_1.second * sizeof(wchar_t));
          plstrClsVersion->MaximumLength = plstrClsVersion->Length + 2;
          plstrClsVersion->bAnsi = FALSE;
        }
      }

      if (reinterpret_cast<SIZE_T>(plstrWindowName) & 0xFFFF0000)
      {

        if (plstrWindowName->bAnsi)
        {
          buffer_2 = Utils::MBCSToWide({ reinterpret_cast<const char*>(plstrWindowName->Buffer), plstrWindowName->Length });
          plstrWindowName->Buffer = buffer_2.first.get();
          plstrWindowName->Length = static_cast<ULONG>(buffer_2.second * sizeof(wchar_t));
          plstrWindowName->MaximumLength = plstrWindowName->Length + (1 * sizeof(wchar_t));
          plstrWindowName->bAnsi = FALSE;
        }
      }

      dwExStyle ^= 0x80000000;
    }

    return Utils::SHooker<NtUserCreateWindowEx_Hook>::FnRaw(dwExStyle, plstrClassName, plstrClsVersion, plstrWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam, dwFlags, acbiBuffer);
  }

  static auto __stdcall NtUserCreateWindowEx_Win32u_Hook(DWORD dwExStyle, PLARGE_STRING plstrClassName, PLARGE_STRING plstrClsVersion, PLARGE_STRING plstrWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam, DWORD dwFlags, PVOID acbiBuffer, DWORD dwUn0, DWORD dwUn1) -> HWND
  {
    std::pair<std::unique_ptr<wchar_t[]>, std::size_t> buffer_0;
    std::pair<std::unique_ptr<wchar_t[]>, std::size_t> buffer_1;
    std::pair<std::unique_ptr<wchar_t[]>, std::size_t> buffer_2;

    if (dwExStyle & 0x80000000)
    {
      if (reinterpret_cast<SIZE_T>(plstrClassName) & 0xFFFF0000)
      {
        if (plstrClassName->bAnsi)
        {
          buffer_0 = Utils::MBCSToWide({ reinterpret_cast<const char*>(plstrClassName->Buffer), plstrClassName->Length });
          plstrClassName->Buffer = buffer_0.first.get();
          plstrClassName->Length = static_cast<ULONG>(buffer_0.second * sizeof(wchar_t));
          plstrClassName->MaximumLength = plstrClassName->Length + (1 * sizeof(wchar_t));
          plstrClassName->bAnsi = FALSE;
        }
      }

      if (reinterpret_cast<SIZE_T>(plstrClsVersion) & 0xFFFF0000)
      {

        if (plstrClsVersion->bAnsi)
        {
          buffer_1 = Utils::MBCSToWide({ reinterpret_cast<const char*>(plstrClsVersion->Buffer), plstrClsVersion->Length });
          plstrClsVersion->Buffer = buffer_1.first.get();
          plstrClsVersion->Length = static_cast<ULONG>(buffer_1.second * sizeof(wchar_t));
          plstrClsVersion->MaximumLength = plstrClsVersion->Length + 2;
          plstrClsVersion->bAnsi = FALSE;
        }
      }

      if (reinterpret_cast<SIZE_T>(plstrWindowName) & 0xFFFF0000)
      {

        if (plstrWindowName->bAnsi)
        {
          buffer_2 = Utils::MBCSToWide({ reinterpret_cast<const char*>(plstrWindowName->Buffer), plstrWindowName->Length });
          plstrWindowName->Buffer = buffer_2.first.get();
          plstrWindowName->Length = static_cast<ULONG>(buffer_2.second * sizeof(wchar_t));
          plstrWindowName->MaximumLength = plstrWindowName->Length + (1 * sizeof(wchar_t));
          plstrWindowName->bAnsi = FALSE;
        }
      }

      dwExStyle ^= 0x80000000;
    }

    return Utils::SHooker<NtUserCreateWindowEx_Win32u_Hook>::FnRaw(dwExStyle, plstrClassName, plstrClsVersion, plstrWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam, dwFlags, acbiBuffer, dwUn0, dwUn1);
  }


  static auto FindNtUserCreateWindowEx() -> std::size_t
  {
    // for windows 10 11
    if (const auto win32u_handle = ::GetModuleHandleW(L"win32u.dll"); win32u_handle != NULL)
    {
      return reinterpret_cast<std::size_t>(::GetProcAddress(win32u_handle, "NtUserCreateWindowEx"));
    }

    // for windows 7 8 
    {

    }

    return 0;
  }

  auto WndPatcher::Install() -> bool
  {
    const auto fn_NtUserCreateWindowEx_va = Core::FindNtUserCreateWindowEx();
    if (fn_NtUserCreateWindowEx_va == 0) { return false; }
    Utils::SHooker<Core::NtUserCreateWindowEx_Win32u_Hook>::AttachAndCommit(fn_NtUserCreateWindowEx_va);
    return true;
  }
} // namespace ZQF::LS::Core
