#include "GdiPatcher.h"
#include <Windows.h>
#include <Locale_Simulator/Utils/SHooker.h>
#include <Locale_Simulator/Utils/LSStatus.h>


namespace ZQF::LS::Core
{
  static std::size_t sg_nCodePageNew;
  static std::size_t sg_nCodePageRaw;
  static auto APIENTRY NtGdiGetCharSet_Hook(HDC hDC) -> WORD
  {
    const auto code_page = Utils::SHooker<NtGdiGetCharSet_Hook>::FnRaw(hDC);
    return (static_cast<std::size_t>(code_page) == sg_nCodePageRaw) ? static_cast<WORD>(sg_nCodePageNew) : static_cast<WORD>(code_page);
  }

  static auto FindNtUserCreateWindowEx() -> std::size_t
  {
    // for windows 10 11
    if (const auto win32u_handle = ::GetModuleHandleW(L"win32u.dll"); win32u_handle != NULL)
    {
      return reinterpret_cast<std::size_t>(::GetProcAddress(win32u_handle, "NtGdiGetCharSet"));
    }

    // for windows 7 8 
    {

    }

    return 0;
  }


  static std::size_t sg_nCharset;
  static auto APIENTRY NtGdiHfontCreate_Hook(PENUMLOGFONTEXDVW pelfw, ULONG cjElfw, DWORD lft, FLONG fl, PVOID pvCliData) -> HFONT
  {
    switch (pelfw->elfEnumLogfontEx.elfLogFont.lfCharSet)
    {
    case ANSI_CHARSET:
    case DEFAULT_CHARSET:
    {
      pelfw->elfEnumLogfontEx.elfLogFont.lfCharSet = static_cast<BYTE>(sg_nCharset);
    }
    break;
    }

    return Utils::SHooker<NtGdiHfontCreate_Hook>::FnRaw(pelfw, cjElfw, lft, fl, pvCliData);
  }

  static auto FindNtGdiHfontCreate() -> std::size_t
  {
    // for windows 10 11
    if (const auto win32u_handle = ::GetModuleHandleW(L"win32u.dll"); win32u_handle != NULL)
    {
      return reinterpret_cast<std::size_t>(::GetProcAddress(win32u_handle, "NtGdiHfontCreate"));
    }

    // for windows 7 8
    {

    }

    return 0;
  }

  auto GdiPatcher::Install() -> bool
  {
    {
      const auto fn_NtGdiGetCharSet_va = Core::FindNtUserCreateWindowEx();
      if (fn_NtGdiGetCharSet_va == 0) { return false; }

      const auto code_page_new_opt = Utils::LSStatus::GetCodePageNew();
      if (!code_page_new_opt.has_value()) { return false; }

      const auto code_page_raw_opt = Utils::LSStatus::GetCodePageRaw();
      if (!code_page_raw_opt.has_value()) { return false; }

      sg_nCodePageNew = *code_page_new_opt;
      sg_nCodePageRaw = *code_page_raw_opt;

      Utils::SHooker<Core::NtGdiGetCharSet_Hook>::AttachAndCommit(fn_NtGdiGetCharSet_va);
    }

    {
      const auto fn_NtNtGdiHfontCreate_va = Core::FindNtGdiHfontCreate();
      if (fn_NtNtGdiHfontCreate_va == 0) { return false; }

      const auto charset_opt = Utils::LSStatus::GetCharset();
      if (!charset_opt.has_value()) { return false; }

      sg_nCharset = *charset_opt;

      Utils::SHooker<Core::NtGdiHfontCreate_Hook>::AttachAndCommit(fn_NtNtGdiHfontCreate_va);
    }


    return true;
  }
} // namespace ZQF::LS::Core
