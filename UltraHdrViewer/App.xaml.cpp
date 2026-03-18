#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"

#include "Core\StartupLogger.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::UltraHdrViewer::implementation
{
    App::App()
    {
        ::UltraHdrViewer::Core::WriteStartupLog(L"App constructor entered");
        InitializeComponent();
        ::UltraHdrViewer::Core::WriteStartupLog(L"App InitializeComponent completed");
    }

    void App::OnLaunched(LaunchActivatedEventArgs const&)
    {
        ::UltraHdrViewer::Core::WriteStartupLog(L"OnLaunched entered");

        try
        {
            m_window = winrt::make<MainWindow>();
            ::UltraHdrViewer::Core::WriteStartupLog(L"MainWindow created");

            m_window.Activate();
            ::UltraHdrViewer::Core::WriteStartupLog(L"MainWindow activated");
        }
        catch (winrt::hresult_error const& error)
        {
            ::UltraHdrViewer::Core::WriteStartupLogHr(L"OnLaunched failed with hresult_error", error.code().value);
            ::UltraHdrViewer::Core::WriteStartupLog(std::wstring(L"Message: ") + error.message().c_str());
            throw;
        }
    }
}
