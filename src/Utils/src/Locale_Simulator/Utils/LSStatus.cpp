#include "LSStatus.h"
#include "EnvHelpher.h"
#include <Windows.h>

namespace ZQF::LS::Utils
{
  auto LSStatus::GetCodePageNew() -> std::optional<std::size_t>
  {
    return Utils::EnvHelper::LoadNum(L"Locale_Simulator.CodePage.New");
  }

  auto LSStatus::GetCodePageRaw() -> std::optional<std::size_t>
  {
    return Utils::EnvHelper::LoadNum(L"Locale_Simulator.CodePage.Raw");
  }

  auto LSStatus::SetCodePage(const std::size_t nCodePageNew, const std::size_t nCodePageSys) -> bool
  {
    const auto status_0 = Utils::EnvHelper::StoreNum(L"Locale_Simulator.CodePage.New", nCodePageNew);
    if (!status_0) { return false; }
    const auto status_1 = Utils::EnvHelper::StoreNum(L"Locale_Simulator.CodePage.Raw", nCodePageSys);
    if (!status_1) { return false; }
    return true;
  }


  auto LSStatus::GetCharset() -> std::optional<std::size_t>
  {
    return Utils::EnvHelper::LoadNum(L"Locale_Simulator.Charset.New");
  }

  auto LSStatus::SetCharset(const std::size_t nCharset) -> bool
  {
    return Utils::EnvHelper::StoreNum(L"Locale_Simulator.Charset.New", nCharset);
  }
}
