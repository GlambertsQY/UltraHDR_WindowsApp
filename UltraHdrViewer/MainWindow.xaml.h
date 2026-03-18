#pragma once

#include "MainWindow.g.h"

#include <memory>

#include "Core\AppState.h"
#include "Host\ViewportHost.h"
#include "Renderer\IRenderBackend.h"

namespace winrt::UltraHdrViewer::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        void OnOpenClicked(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void OnFitClicked(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void OnActualSizeClicked(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void OnHdrToggleClicked(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        HWND GetWindowHandle();
        void OnWindowLoaded(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void OnWindowClosed(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::WindowEventArgs const& args);
        void OnWindowSizeChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::SizeChangedEventArgs const& args);
        void UpdateViewportBounds();
        void UpdateUiState();

        ::UltraHdrViewer::Core::AppState m_state{};
        std::unique_ptr<::UltraHdrViewer::Renderer::IRenderBackend> m_backend;
        std::unique_ptr<::UltraHdrViewer::Host::ViewportHost> m_viewportHost;
    };
}

namespace winrt::UltraHdrViewer::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
