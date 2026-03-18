#pragma once

#include "App.xaml.g.h"

namespace winrt::UltraHdrViewer::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const& args);

    private:
        winrt::UltraHdrViewer::MainWindow m_window{ nullptr };
    };
}
