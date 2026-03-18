#include "pch.h"
#include "StartupLogger.h"

#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>

namespace UltraHdrViewer::Core
{
    namespace
    {
        std::mutex g_logMutex;

        std::wstring LogPath()
        {
            std::wstring path(MAX_PATH, L'\0');
            const auto length = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
            path.resize(length);

            const auto slash = path.find_last_of(L"\\/");
            if (slash != std::wstring::npos)
            {
                path.resize(slash + 1);
            }
            else
            {
                path.clear();
            }

            path += L"startup.log";
            return path;
        }

        std::wstring Timestamp()
        {
            SYSTEMTIME localTime{};
            GetLocalTime(&localTime);

            std::wostringstream stream;
            stream
                << std::setfill(L'0')
                << std::setw(4) << localTime.wYear << L'-'
                << std::setw(2) << localTime.wMonth << L'-'
                << std::setw(2) << localTime.wDay << L' '
                << std::setw(2) << localTime.wHour << L':'
                << std::setw(2) << localTime.wMinute << L':'
                << std::setw(2) << localTime.wSecond << L'.'
                << std::setw(3) << localTime.wMilliseconds;
            return stream.str();
        }
    }

    void ResetStartupLog()
    {
        std::scoped_lock lock(g_logMutex);
        std::wofstream stream(LogPath(), std::ios::trunc);
        if (stream)
        {
            stream << L"==== UltraHdrViewer startup log ====\n";
        }
    }

    void WriteStartupLog(std::wstring_view message)
    {
        std::scoped_lock lock(g_logMutex);
        std::wofstream stream(LogPath(), std::ios::app);
        if (!stream)
        {
            return;
        }

        stream << L'[' << Timestamp() << L"] " << message << L"\n";
    }

    void WriteStartupLogHr(std::wstring_view context, HRESULT hr)
    {
        std::wostringstream stream;
        stream << context << L" hr=0x" << std::uppercase << std::hex << static_cast<std::uint32_t>(hr);
        WriteStartupLog(stream.str());
    }
}
