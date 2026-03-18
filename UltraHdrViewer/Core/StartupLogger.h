#pragma once

#include <string_view>

namespace UltraHdrViewer::Core
{
    void ResetStartupLog();
    void WriteStartupLog(std::wstring_view message);
    void WriteStartupLogHr(std::wstring_view context, HRESULT hr);
}
