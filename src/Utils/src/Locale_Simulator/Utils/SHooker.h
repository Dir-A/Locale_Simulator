#pragma once
#include <cstddef>


// SingleHooker
namespace ZQF::LS::Utils
{
  class SHookerDetour
  {
  public:
    static auto CommitBeg() -> void;
    static auto CommitEnd() -> void;
    static auto PushAttach(void* ppFunc, void* pDetour) -> void;
    static auto PushDetach(void* ppFunc, void* pDetour) -> void;
    static auto AfterWith() -> void;
  };

  template<auto FnDetour>
  class SHooker
  {
  public:
    using FnType_t = decltype(FnDetour);

  public:
    inline static FnType_t FnRaw;

  public:
    static auto PushAttach(SHooker::FnType_t pRawFunc) -> void
    {
      SHooker::FnRaw = pRawFunc;
      SHookerDetour::PushAttach(&FnRaw, FnDetour);
    }

    static auto PushAttach(const std::size_t nRawFuncVA) -> void
    {
      SHooker::PushAttach(reinterpret_cast<SHooker::FnType_t>(nRawFuncVA));
    }

    static auto PushAttach(const std::size_t nImageBase, const std::size_t nRva) -> void
    {
      SHooker::PushAttach(reinterpret_cast<SHooker::FnType_t>(nImageBase + nRva));
    }

    static auto PushDetach() -> void
    {
      SHookerDetour::PushDetach(&FnRaw, FnDetour);
    }

    static auto AttachAndCommit(SHooker::FnType_t pRawFunc) -> void
    {
      SHookerDetour::CommitBeg();
      SHooker::PushAttach(pRawFunc);
      SHookerDetour::CommitEnd();
    }

    static auto AttachAndCommit(const std::size_t nRawFuncVA) -> void
    {
      SHooker::AttachAndCommit(reinterpret_cast<SHooker::FnType_t>(nRawFuncVA));
    }

    static auto AttachAndCommit(const std::size_t nImageBase, const std::size_t nRva) -> void
    {
      SHooker::AttachAndCommit(reinterpret_cast<SHooker::FnType_t>(nImageBase + nRva));
    }

    static auto DetachAndCommit() -> void
    {
      SHookerDetour::CommitBeg();
      SHooker::PushDetach();
      SHookerDetour::CommitEnd();
    }
  };
} // namespace ZQF::LS::Utils
