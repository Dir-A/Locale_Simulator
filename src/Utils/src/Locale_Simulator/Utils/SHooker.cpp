#include "SHooker.h"
#include <Windows.h>
#include <detours.h>


namespace ZQF::LS::Utils
{
  auto SHookerDetour::CommitBeg() -> void
  {
    ::DetourTransactionBegin();
    ::DetourUpdateThread(GetCurrentThread());
  }

  auto SHookerDetour::CommitEnd() -> void
  {
    ::DetourTransactionCommit();
  }

  auto SHookerDetour::PushAttach(void* ppFunc, void* pDetour) -> void
  {
    ::DetourAttach((PVOID*)ppFunc, pDetour);
  }

  auto SHookerDetour::PushDetach(void* ppFunc, void* pDetour) -> void
  {
    ::DetourDetach((PVOID*)ppFunc, pDetour);
  }

  auto SHookerDetour::AfterWith() -> void
  {
    ::DetourRestoreAfterWith();
  }
} // namespace ZQF::LS::Utils
