#include "GdiPatcher.h"
#include <Windows.h>
#include <ZxHook/SHooker.h>
#include <Locale_Simulator/Utils/EnvHelpher.h>


namespace ZQF::LS::Core
{
    static std::size_t sg_nCodePageOrg;
    static std::size_t sg_nCodePageSet;
    static auto APIENTRY NtGdiGetCharSet_Hook(HDC hDC) -> WORD
    {
        const auto code_page = ZxHook::SHooker<NtGdiGetCharSet_Hook>::FnRaw(hDC);
        return (static_cast<std::size_t>(code_page) == sg_nCodePageOrg) ? static_cast<WORD>(sg_nCodePageSet) : static_cast<WORD>(code_page);
    }


    static auto FindNtUserCreateWindowEx() -> std::size_t
    {
        if (const auto win32u_handle = ::GetModuleHandleW(L"win32u.dll"); win32u_handle != NULL)
        {
            return reinterpret_cast<std::size_t>(::GetProcAddress(win32u_handle, "NtGdiGetCharSet"));
        }

        // windows7 8 8.1 find NtGdiGetCharSet
        {

        }

        return 0;
    }

    static std::size_t sg_nCharSetSet;
    static auto APIENTRY NtGdiHfontCreate_Hook(PENUMLOGFONTEXDVW pelfw, ULONG cjElfw, DWORD lft, FLONG fl, PVOID pvCliData) -> HFONT
    {
        switch (pelfw->elfEnumLogfontEx.elfLogFont.lfCharSet)
        {
        case ANSI_CHARSET:
        case DEFAULT_CHARSET:
        {
            pelfw->elfEnumLogfontEx.elfLogFont.lfCharSet = static_cast<BYTE>(sg_nCharSetSet);
        }
        break;
        }
        
        return ZxHook::SHooker<NtGdiHfontCreate_Hook>::FnRaw(pelfw, cjElfw, lft, fl, pvCliData);
    }

    static auto FindNtGdiHfontCreate() -> std::size_t
    {
        if (const auto win32u_handle = ::GetModuleHandleW(L"win32u.dll"); win32u_handle != NULL)
        {
            return reinterpret_cast<std::size_t>(::GetProcAddress(win32u_handle, "NtGdiHfontCreate"));
        }

        // windows7 8 8.1 find NtGdiHfontCreate
        {

        }

        return 0;
    }

    auto GdiPatcher::Install() -> bool
    {
        {
            const auto fn_NtGdiGetCharSet_va = Core::FindNtUserCreateWindowEx();
            if (fn_NtGdiGetCharSet_va == 0) { return false; }

            const auto code_page_org_opt = Utils::EnvHelper::LoadNum(L"Locale_Simulator_CodePage_Org");
            if (!code_page_org_opt.has_value()) { return false; }

            const auto code_page_set_opt = Utils::EnvHelper::LoadNum(L"Locale_Simulator_CodePage_Set");
            if (!code_page_set_opt.has_value()) { return false; }

            sg_nCodePageOrg = *code_page_org_opt;
            sg_nCodePageSet = *code_page_set_opt;

            ZxHook::SHooker<Core::NtGdiGetCharSet_Hook>::Commit(fn_NtGdiGetCharSet_va);
        }

        {
            const auto fn_NtNtGdiHfontCreate_va = Core::FindNtGdiHfontCreate();
            if (fn_NtNtGdiHfontCreate_va == 0) { return false; }

            const auto charset_set_opt = Utils::EnvHelper::LoadNum(L"Locale_Simulator_CharSet_Set");
            if (!charset_set_opt.has_value()) { return false; }

            sg_nCharSetSet = *charset_set_opt;

            ZxHook::SHooker<Core::NtGdiHfontCreate_Hook>::Commit(fn_NtNtGdiHfontCreate_va);
        }


        return true;
    }
} // namespace ZQF::LS::Core
