#include "NlsPatcher.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <Locale_Simulator/Utils/VAHelper.h>
#include <Locale_Simulator/Utils/SysInfo.h>
#include <Locale_Simulator/Utils/EnvHelpher.h>
#include <Locale_Simulator/Utils/LSStatus.h>


namespace ZQF::LS::Core
{
#ifndef _WIN64
  // for Windows 7 8 8.1 10  x32
  static auto ShellCodeV1_x32(uint8_t* pBuffer) -> std::size_t
  {
    /*
    ** _PEB* peb_ptr = __readfsdword(0x30);
    ** SIZE_T code_page_data_size;
    ** NtGetNlsSectionPtr(0xB, 0x3A4, 0, &peb_ptr->AnsiCodePageData, &code_page_data_size);
    ** peb_ptr->OemCodePageData = peb_ptr->AnsiCodePageData
    */

    std::array<std::uint8_t, 104> shell_code
    {
        0x60,                                     // pushad
        0x9C,                                     // pushfd
        0xE8, 0x10,0x00,0x00,0x00,                // call lable_0

        // Data
        0x00,0x00,0x00,0x00,                      // info_struct.CodePage
        0x00,0x00,0x00,0x00,                      // info_struct.NtGetNlsSectionPtr
        0x00,0x00,0x00,0x00,                      // info_struct.RtlInitCodePageTable
        0x00,0x00,0x00,0x00,                      // info_struct.NlsMapVA

        // lable_0: test info_struct.NlsMapVA
        0x58,                                     // pop eax
        0x31,0xDB,                                // xor ebx,ebx
        0x8B,0x48,0x0C,                           // mov ecx,dword ptr ds:[eax+C]   // info_struct.NlsMapVA
        0x39,0xCB,                                // cmp ebx,ecx
        0x75,0x2F,                                // jne label_1

        0x50,                                     // push eax                        // Save info_struct ptr
        0x8B,0x18,                                // mov ebx,dword ptr ds:[eax]      // Load info_struct.CodePage

        // load nls map table
        0x6A,0x00,                                // push 0
        0x6A,0x00,                                // push 0
        0x8D,0x0C,0x24,                           // lea ecx,dword ptr ss:[esp]
        0x8D,0x54,0x24,0x04,                      // lea edx,dword ptr ss:[esp+4]
        0x51,                                     // push ecx                        // param_4: SectionSizePtr :
        0x52,                                     // push edx                        // param_3: SectionPtrPtr  :
        0x6A,0x00,                                // push 0                          // param_2: Unknown        :
        0x53,                                     // push ebx                        // param_1: CodePage       :
        0x6A,0x0B,                                // push B                          // param_0: NlsType        :
        0xFF,0x50,0x04,                           // call dword ptr ds:[eax+4]       // call info_struct.NtGetNlsSectionPtr
        0x5B,                                     // pop ebx
        0x59,                                     // pop ecx
        0x58,                                     // pop eax                         // Restore info_struct ptr

        // patch PEB
        0x64,0x8B,0x35, 0x30,0x00,0x00,0x00,      // mov esi,dword ptr fs:[30]       // Get PEB
        0x8D,0x56,0x58,                           // lea edx,dword ptr ds:[esi+58]   // &PEB.AnsiCodePageData
        0x89,0x0A,                                // mov dword ptr ds:[edx],ecx      // PEB.AnsiCodePageData = nls_table_ptr
        0x89,0x4A,0x04,                           // mov dword ptr ds:[edx+4],ecx    // PEB.OemCodePageData = nls_table_ptr
        0x89,0x48,0x0C,                           // mov dword ptr ds:[eax+C],ecx    // info_struct.NlsMapVA = nls_table_ptr
        0xEB,0x0D,                                // jmp label_2

        // label_1: restore RtlInitCodePageTable header
        0x8B,0x58,0x08,                           // mov ebx,dword ptr ds:[eax+8]    // info_struct.RtlInitCodePageTable
        0xC7,0x03, 0x8B,0xFF,0x55,0x8B,           // mov dword ptr ds:[ebx],8B55FF8B // Copy { {mov edi, edi}, {push ebp}, {mov ebp, esp} } to RtlInitCodePageTable
        0xC6,0x43,0x04, 0xEC,                     // mov byte ptr ds:[ebx+4],EC

        // label_2: modify RtlInitCodePageTable param0
        0x89,0x4C,0x24,0x28,                      // mov dword ptr ss:[esp+28],ecx
        0x9D,                                     // popfd
        0x61,                                     // popad
        0x8B, 0xFF,                               // mov edi, edi
        0x55,                                     // push ebp
        0x8B, 0xEC                                // mov ebp, esp
    };

    std::memcpy(pBuffer, shell_code.data(), shell_code.size());
    return shell_code.size();
  }

  // for Windows 11  x32
  static auto ShellCodeV2_x32(uint8_t* pBuffer) -> std::size_t
  {
    /*
    *  PEB* peb_ptr = __readfsdword(0x30);
    *  peb_ptr->ActiveCodePage = 0x3A4;
    *  peb_ptr->OemCodePage = 0x3A4;
    *  SIZE_T code_page_table_ptr;
    *  SIZE_T code_page_table_size;
    *  NtGetNlsSectionPtr(0xB, 0x3A4, 0, &code_page_table_ptr, &code_page_table_size);
    *  RtlInitCodePageTable(code_page_table_ptr, &GlobalRtlNlsState);
    */
    std::array<std::uint8_t, 110> shell_code
    {
        0x60,                                     // pushad
        0x9C,                                     // pushfd
        0xE8, 0x10,0x00,0x00,0x00,                // call lable_0

        // Data
        0x00,0x00,0x00,0x00,                      // info_struct.CodePage
        0x00,0x00,0x00,0x00,                      // info_struct.NtGetNlsSectionPtr
        0x00,0x00,0x00,0x00,                      // info_struct.RtlInitCodePageTable
        0x00,0x00,0x00,0x00,                      // info_struct.NlsMapVA

        // lable_0: test info_struct.NlsMapVA
        0x58,                                     // pop eax
        0x31,0xDB,                                // xor ebx,ebx
        0x8B,0x48,0x0C,                           // mov ecx,dword ptr ds:[eax+C]   // info_struct.NlsMapVA
        0x39,0xCB,                                // cmp ebx,ecx
        0x75,0x35,                                // jne label_1

        // patch PEB
        0x50,                                     // push eax                        // Save info_struct ptr
        0x8B,0x18,                                // mov ebx,dword ptr ds:[eax]      // info_struct.CodePage
        0x64,0x8B,0x15, 0x30,0x00,0x00,0x00,      // mov edx,dword ptr fs:[30]       // Get PEB
        0x66,0x89,0x9A,0x28,0x02,0x00,0x00,       // mov word ptr ds:[edx+228],bx    // PEB.ActiveCodePage
        0x66,0x89,0x9A,0x2A,0x02,0x00,0x00,       // mov word ptr ds:[edx+22A],bx    // PEB.OemCodePage

        // load nls map table
        0x6A,0x00,                                // push 0
        0x6A,0x00,                                // push 0
        0x8D,0x0C,0x24,                           // lea ecx,dword ptr ss:[esp]
        0x8D,0x54,0x24,0x04,                      // lea edx,dword ptr ss:[esp+4]
        0x51,                                     // push ecx                        // param_4: SectionSizePtr :
        0x52,                                     // push edx                        // param_3: SectionPtrPtr  :
        0x6A,0x00,                                // push 0                          // param_2: Unknown        :
        0x53,                                     // push ebx                        // param_1: CodePage       :
        0x6A,0x0B,                                // push B                          // param_0: NlsType        :
        0xFF,0x50,0x04,                           // call dword ptr ds:[eax+4]       // call info_struct.NtGetNlsSectionPtr
        0x5B,                                     // pop ebx
        0x59,                                     // pop ecx
        0x58,                                     // pop eax                         // Restore info_struct ptr
        0x89,0x48,0x0C,                           // mov dword ptr ds:[eax+C],ecx
        0xEB,0x0D,                                // jmp label_2

        // label_1: restore RtlInitCodePageTable header
        0x8B,0x58,0x08,                           // mov ebx,dword ptr ds:[eax+8]    // info_struct.RtlInitCodePageTable
        0xC7,0x03, 0x8B,0xFF,0x55,0x8B,           // mov dword ptr ds:[ebx],8B55FF8B // Copy { {mov edi, edi}, {push ebp}, {mov ebp, esp} } to RtlInitCodePageTable
        0xC6,0x43,0x04, 0xEC,                     // mov byte ptr ds:[ebx+4],EC

        // label_2: modify RtlInitCodePageTable param0
        0x89,0x4C,0x24,0x28,                      // mov dword ptr ss:[esp+28],ecx
        0x9D,                                     // popfd
        0x61,                                     // popad
        0x8B, 0xFF,                               // mov edi, edi
        0x55,                                     // push ebp
        0x8B, 0xEC                                // mov ebp, esp
    };

    std::memcpy(pBuffer, shell_code.data(), shell_code.size());
    return shell_code.size();
  }

  static auto CodePageTablePatch(const HANDLE hProcess, const std::size_t nCodePage, const bool isWin11) -> bool
  {
    // find NtGetNlsSectionPtr and RtlInitCodePageTable
    const auto ntdll_handle = ::GetModuleHandleW(L"ntdll.dll");
    if (ntdll_handle == NULL) { return false; }
    const auto fn_NtGetNlsSectionPtr_va = reinterpret_cast<std::size_t>(::GetProcAddress(ntdll_handle, "NtGetNlsSectionPtr"));
    if (fn_NtGetNlsSectionPtr_va == 0) { return false; }
    const auto fn_RtlInitCodePageTable_va = reinterpret_cast<std::size_t>(::GetProcAddress(ntdll_handle, "RtlInitCodePageTable"));
    if (fn_RtlInitCodePageTable_va == 0) { return false; }

    // get shellcode
    std::uint8_t shell_code_buffer[512];
    const auto shell_code_bytes = isWin11 ? Core::ShellCodeV2_x32(shell_code_buffer) : Core::ShellCodeV1_x32(shell_code_buffer);
    const std::span<std::uint8_t> shell_code{ shell_code_buffer, shell_code_bytes };

    // create va_helper
    Utils::VAHelper va_helper{ hProcess };

    // alloc shellcode memory in target process
    const auto shell_code_mem_va = va_helper.Alloc(1024);
    if (shell_code_mem_va == 0) { return false; }

    // config shellcode
    {
      const auto info_strcut_ptr = reinterpret_cast<std::uint32_t*>(shell_code.data() + 7);
      info_strcut_ptr[0] = static_cast<const std::uint32_t>(nCodePage);
      info_strcut_ptr[1] = static_cast<const std::uint32_t>(fn_NtGetNlsSectionPtr_va);
      info_strcut_ptr[2] = static_cast<const std::uint32_t>(fn_RtlInitCodePageTable_va);
      info_strcut_ptr[3] = {};
    }

    // write shellcode
    {
      // write shellcode to shell_code_mem
      const auto shellcode_write_status = va_helper.Write(shell_code_mem_va, shell_code);
      if (shellcode_write_status == false) { return false; }

      // modify RtlInitNlsTables va protection properties
      const auto old_protection = va_helper.SetProtect(fn_RtlInitCodePageTable_va, 5, PAGE_EXECUTE_READWRITE);
      if (old_protection == 0) { return false; }

      // write jmp instr at the begin of NtRtlInitNlsTables
      const auto jmp_instr_write_status = va_helper.WriteAsmJmp(fn_RtlInitCodePageTable_va, shell_code_mem_va);
      if (jmp_instr_write_status == false) { return false; }

      // write jmp instr at the end of shell_code_mem that jmp back begin of NtRtlInitNlsTables
      const auto jmp_back_instr_write_status = va_helper.WriteAsmJmp(shell_code_mem_va + shell_code.size(), fn_RtlInitCodePageTable_va + 5);
      if (jmp_back_instr_write_status == false) { return false; }

      return Core::NlsPatcher::SaveInfoViaEnv(fn_RtlInitCodePageTable_va, shell_code_mem_va, old_protection);
    }
  }
#else

#endif // _WIN64


} // namespace ZQF::LS::Core

namespace ZQF::LS::Core
{
  auto NlsPatcher::Install(const HANDLE hProcess) -> bool
  {
    const auto code_page_new_opt = Utils::LSStatus::GetCodePageNew();
    if (!code_page_new_opt.has_value()) { return false; }
    const auto is_win11 = (Utils::SysInfo::GetBuildNumber() >= 22000);
    return CodePageTablePatch(hProcess, *code_page_new_opt, is_win11);
  }

  auto NlsPatcher::AfterWith() -> bool
  {
    const auto hook_va_opt = Utils::EnvHelper::LoadNum(L"Locale_Simulator.NlsPatcher.HookFunc.VA");
    if (!hook_va_opt.has_value()) { return false; }
    const auto shellcode_va_opt = Utils::EnvHelper::LoadNum(L"Locale_Simulator.NlsPatcher.Shellcode.VA");
    if (!shellcode_va_opt.has_value()) { return false; }
    const auto old_protect_opt = Utils::EnvHelper::LoadNum(L"Locale_Simulator.NlsPatcher.HookFunc.Protect");
    if (!old_protect_opt.has_value()) { return false; }

    Utils::VAHelper va_helper{ ::GetCurrentProcess() };

    const auto free_status = va_helper.Free(*shellcode_va_opt);
    if (!free_status) { return false; }
    const auto set_protect_status = va_helper.SetProtect(*hook_va_opt, 5, *old_protect_opt);
    if (!set_protect_status) { return false; }

    return true;
  }

  auto NlsPatcher::SaveInfoViaEnv(const std::size_t nHookVA, const std::size_t nShellCodeVA, const std::size_t nOldProtect) -> bool
  {
    const auto store_status_0 = Utils::EnvHelper::StoreNum(L"Locale_Simulator.NlsPatcher.HookFunc.VA", nHookVA);
    if (!store_status_0) { return false; }
    const auto store_status_1 = Utils::EnvHelper::StoreNum(L"Locale_Simulator.NlsPatcher.Shellcode.VA", nShellCodeVA);
    if (!store_status_1) { return false; }
    const auto store_status_2 = Utils::EnvHelper::StoreNum(L"Locale_Simulator.NlsPatcher.HookFunc.Protect", nOldProtect);
    if (!store_status_2) { return false; }
    return true;
  }
} // namespace ZQF::LS::Core
