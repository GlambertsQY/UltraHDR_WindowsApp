#include "pch.h"
#include "MainWindow.xaml.h"
#include "MainWindow.xaml.g.hpp"

#include "Renderer\RenderBackendFactory.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::UltraHdrViewer::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        this->Title(L"Ultra HDR Viewer");
        ContentRoot().Loaded({ this, &MainWindow::OnWindowLoaded });
        ContentRoot().SizeChanged({ this, &MainWindow::OnWindowSizeChanged });
        Closed({ this, &MainWindow::OnWindowClosed });

        m_state.activeBackend = ::UltraHdrViewer::Renderer::RenderBackendKind::Vulkan;
        m_state.sessionMessage = L"Scaffold ready. File picker and image loading are next.";
    }

    void MainWindow::InitializeViewportHost()
    {
    }

    void MainWindow::OnOpenClicked(IInspectable const&, RoutedEventArgs const&)
    {
        m_state.sessionMessage = L"Open command placeholder: file picker wiring is scheduled for the next milestone.";
        UpdateUiState();
    }

    void MainWindow::OnFitClicked(IInspectable const&, RoutedEventArgs const&)
    {
        StatusBarText().Text(L"Fit-to-window command scaffolded. View state logic will be connected after image loading lands.");
    }

    void MainWindow::OnActualSizeClicked(IInspectable const&, RoutedEventArgs const&)
    {
        StatusBarText().Text(L"Actual-size command scaffolded. Pixel-perfect zoom will be connected once textures are uploaded.");
    }

    void MainWindow::OnHdrToggleClicked(IInspectable const&, RoutedEventArgs const&)
    {
        m_state.preferHdr = !m_state.preferHdr;
        UpdateUiState();
    }

    HWND MainWindow::GetWindowHandle()
    {
        auto windowNative{ this->m_inner.as<IWindowNative>() };
        HWND hwnd{};
        windowNative->get_WindowHandle(&hwnd);
        return hwnd;
    }

    void MainWindow::OnWindowLoaded(IInspectable const&, RoutedEventArgs const&)
    {
        if (!m_backend)
        {
            m_backend = ::UltraHdrViewer::Renderer::CreateRenderBackend(m_state.activeBackend);
        }

        if (!m_viewportHost)
        {
            m_viewportHost = std::make_unique<::UltraHdrViewer::Host::ViewportHost>();
            m_viewportHost->Create(GetWindowHandle(), m_backend.get());
        }

        UpdateViewportBounds();
        UpdateUiState();
    }

    void MainWindow::OnWindowClosed(IInspectable const&, WindowEventArgs const&)
    {
        if (m_viewportHost)
        {
            m_viewportHost->Destroy();
        }
    }

    void MainWindow::OnWindowSizeChanged(IInspectable const&, SizeChangedEventArgs const&)
    {
        UpdateViewportBounds();
    }

    void MainWindow::UpdateViewportBounds()
    {
        if (!m_viewportHost || !ViewportPlaceholder().IsLoaded())
        {
            return;
        }

        auto transform = ViewportPlaceholder().TransformToVisual(ContentRoot());
        auto origin = transform.TransformPoint({ 0.0f, 0.0f });
        auto scale = ViewportPlaceholder().XamlRoot().RasterizationScale();

        RECT bounds{};
        bounds.left = static_cast<LONG>(std::lround(origin.X * scale));
        bounds.top = static_cast<LONG>(std::lround(origin.Y * scale));
        bounds.right = bounds.left + static_cast<LONG>(std::lround(ViewportPlaceholder().ActualWidth() * scale));
        bounds.bottom = bounds.top + static_cast<LONG>(std::lround(ViewportPlaceholder().ActualHeight() * scale));

        m_viewportHost->SetBounds(bounds);
    }

    void MainWindow::UpdateUiState()
    {
        if (m_backend)
        {
            BackendStatusText().Text(m_backend->Name());

            std::wstring viewportDescription = m_viewportHost ? m_viewportHost->Describe() : std::wstring(L"Native viewport pending");
            ViewportStatusText().Text(winrt::hstring{ viewportDescription });
            DisplayStatusText().Text(m_state.preferHdr ? L"HDR preference enabled. Display capability probing still needs implementation." : L"HDR preference disabled. SDR-safe path is active until capability probing is added.");
        }

        SessionStatusText().Text(winrt::hstring{ m_state.sessionMessage });
        StatusBarText().Text(m_state.preferHdr ? L"HDR preference is ON. Vulkan backend is currently a stub backend placeholder." : L"HDR preference is OFF. Vulkan backend is currently a stub backend placeholder.");
    }
}
