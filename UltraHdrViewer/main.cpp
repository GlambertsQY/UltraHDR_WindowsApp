#include "pch.h"

#include "App.xaml.h"

#include <cstring>

#include "Core\StartupLogger.h"

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    ::UltraHdrViewer::Core::ResetStartupLog();
    ::UltraHdrViewer::Core::WriteStartupLog(L"wWinMain entered");

    try
    {
        winrt::init_apartment(winrt::apartment_type::single_threaded);
        ::UltraHdrViewer::Core::WriteStartupLog(L"COM apartment initialized");

        winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
        {
            ::UltraHdrViewer::Core::WriteStartupLog(L"Application::Start callback entered");
            winrt::make<winrt::UltraHdrViewer::implementation::App>();
            ::UltraHdrViewer::Core::WriteStartupLog(L"App instance created");
        });

        ::UltraHdrViewer::Core::WriteStartupLog(L"Application::Start returned normally");
    }
    catch (winrt::hresult_error const& error)
    {
        ::UltraHdrViewer::Core::WriteStartupLogHr(L"Startup failed with hresult_error", error.code().value);
        ::UltraHdrViewer::Core::WriteStartupLog(std::wstring(L"Message: ") + error.message().c_str());
        return static_cast<int>(error.code().value);
    }
    catch (std::exception const& error)
    {
        std::wstring message(error.what(), error.what() + std::strlen(error.what()));
        ::UltraHdrViewer::Core::WriteStartupLog(std::wstring(L"Startup failed with std::exception: ") + message);
        return -1;
    }
    catch (...)
    {
        ::UltraHdrViewer::Core::WriteStartupLog(L"Startup failed with unknown exception");
        return -2;
    }

    return 0;
}
