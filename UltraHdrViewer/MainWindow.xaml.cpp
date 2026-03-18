#include "pch.h"
#include "MainWindow.xaml.h"
#include "MainWindow.xaml.g.hpp"

#include <array>
#include <commdlg.h>
#include <optional>
#include <vector>
#include <wincodec.h>

#include "Renderer\RenderBackendFactory.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace
{
    struct ImageProbeResult
    {
        std::uint32_t width{};
        std::uint32_t height{};
        std::wstring pixelFormat;
    };

    struct LoadedImageResult
    {
        ImageProbeResult probe;
        ::UltraHdrViewer::Renderer::ImageData image;
    };

    std::wstring DescribePixelFormat(WICPixelFormatGUID const& format)
    {
        if (format == GUID_WICPixelFormat24bppBGR)
        {
            return L"24bpp BGR";
        }
        if (format == GUID_WICPixelFormat24bppRGB)
        {
            return L"24bpp RGB";
        }
        if (format == GUID_WICPixelFormat32bppBGRA)
        {
            return L"32bpp BGRA";
        }
        if (format == GUID_WICPixelFormat32bppRGBA)
        {
            return L"32bpp RGBA";
        }
        if (format == GUID_WICPixelFormat32bppBGR)
        {
            return L"32bpp BGR";
        }
        if (format == GUID_WICPixelFormat32bppRGB)
        {
            return L"32bpp RGB";
        }
        if (format == GUID_WICPixelFormat8bppGray)
        {
            return L"8bpp Gray";
        }
        if (format == GUID_WICPixelFormat64bppRGBAHalf)
        {
            return L"64bpp RGBA Half";
        }
        if (format == GUID_WICPixelFormat128bppRGBAFloat)
        {
            return L"128bpp RGBA Float";
        }

        wchar_t buffer[64]{};
        if (StringFromGUID2(format, buffer, static_cast<int>(std::size(buffer))) > 0)
        {
            return buffer;
        }

        return L"Unknown";
    }

    std::optional<std::wstring> PickImageFile(HWND owner)
    {
        std::array<wchar_t, MAX_PATH> pathBuffer{};

        OPENFILENAMEW dialog{};
        dialog.lStructSize = sizeof(dialog);
        dialog.hwndOwner = owner;
        dialog.lpstrFile = pathBuffer.data();
        dialog.nMaxFile = static_cast<DWORD>(pathBuffer.size());
        dialog.lpstrFilter = L"Image Files\0*.jpg;*.jpeg;*.png;*.bmp;*.tif;*.tiff;*.gif;*.hdr\0All Files\0*.*\0";
        dialog.nFilterIndex = 1;
        dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (!GetOpenFileNameW(&dialog))
        {
            return std::nullopt;
        }

        return std::wstring{ pathBuffer.data() };
    }

    ImageProbeResult ProbeImageFile(std::wstring const& path)
    {
        ImageProbeResult result{};

        winrt::com_ptr<IWICImagingFactory> imagingFactory;
        winrt::check_hresult(CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(imagingFactory.put())));

        winrt::com_ptr<IWICBitmapDecoder> decoder;
        winrt::check_hresult(imagingFactory->CreateDecoderFromFilename(
            path.c_str(),
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnDemand,
            decoder.put()));

        winrt::com_ptr<IWICBitmapFrameDecode> frame;
        winrt::check_hresult(decoder->GetFrame(0, frame.put()));

        WICPixelFormatGUID pixelFormat{};
        winrt::check_hresult(frame->GetSize(&result.width, &result.height));
        winrt::check_hresult(frame->GetPixelFormat(&pixelFormat));

        result.pixelFormat = DescribePixelFormat(pixelFormat);
        return result;
    }

    LoadedImageResult LoadImageFile(std::wstring const& path)
    {
        LoadedImageResult result{};

        winrt::com_ptr<IWICImagingFactory> imagingFactory;
        winrt::check_hresult(CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(imagingFactory.put())));

        winrt::com_ptr<IWICBitmapDecoder> decoder;
        winrt::check_hresult(imagingFactory->CreateDecoderFromFilename(
            path.c_str(),
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            decoder.put()));

        winrt::com_ptr<IWICBitmapFrameDecode> frame;
        winrt::check_hresult(decoder->GetFrame(0, frame.put()));

        WICPixelFormatGUID sourcePixelFormat{};
        winrt::check_hresult(frame->GetSize(&result.probe.width, &result.probe.height));
        winrt::check_hresult(frame->GetPixelFormat(&sourcePixelFormat));
        result.probe.pixelFormat = DescribePixelFormat(sourcePixelFormat);

        winrt::com_ptr<IWICFormatConverter> converter;
        winrt::check_hresult(imagingFactory->CreateFormatConverter(converter.put()));
        winrt::check_hresult(converter->Initialize(
            frame.get(),
            GUID_WICPixelFormat32bppBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeCustom));

        result.image.width = result.probe.width;
        result.image.height = result.probe.height;
        result.image.stride = result.image.width * 4;
        result.image.pixelFormat = L"32bpp BGRA";
        result.image.pixels.resize(static_cast<size_t>(result.image.stride) * static_cast<size_t>(result.image.height));

        winrt::check_hresult(converter->CopyPixels(
            nullptr,
            result.image.stride,
            static_cast<UINT>(result.image.pixels.size()),
            result.image.pixels.data()));

        return result;
    }
}

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

    void MainWindow::OnOpenClicked(IInspectable const&, RoutedEventArgs const&)
    {
        auto filePath = PickImageFile(GetWindowHandle());
        if (!filePath)
        {
            m_state.sessionMessage = m_state.hasImage ? L"Open cancelled. Keeping the current image metadata." : L"Open cancelled. No image selected.";
            UpdateUiState();
            return;
        }

        try
        {
            auto loaded = LoadImageFile(*filePath);
            m_state.hasImage = true;
            m_state.imagePath = *filePath;
            m_state.pixelWidth = loaded.probe.width;
            m_state.pixelHeight = loaded.probe.height;
            m_state.pixelFormat = loaded.probe.pixelFormat;

            if (m_backend)
            {
                m_backend->UploadImage(std::move(loaded.image));
            }

            if (m_viewportHost)
            {
                m_viewportHost->Invalidate();
            }

            m_state.sessionMessage = L"Loaded image metadata and pushed a software preview into the viewport.";
        }
        catch (winrt::hresult_error const& error)
        {
            m_state.hasImage = false;
            m_state.imagePath = *filePath;
            m_state.pixelWidth = 0;
            m_state.pixelHeight = 0;
            m_state.pixelFormat = L"Unknown";
            if (m_backend)
            {
                m_backend->ClearImage();
            }
            if (m_viewportHost)
            {
                m_viewportHost->Invalidate();
            }
            m_state.sessionMessage = std::wstring(L"Selected file, but metadata decode failed: ") + error.message().c_str();
        }

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

        if (m_state.hasImage)
        {
            std::wstring dimensions = L"Dimensions: ";
            dimensions += std::to_wstring(m_state.pixelWidth);
            dimensions += L" x ";
            dimensions += std::to_wstring(m_state.pixelHeight);

            std::wstring pixelFormat = L"Pixel format: ";
            pixelFormat += m_state.pixelFormat;

            std::wstring status = m_state.preferHdr ? L"HDR preference is ON. Software viewport preview is active while Vulkan presentation is still pending." : L"HDR preference is OFF. Software viewport preview is active while Vulkan presentation is still pending.";

            ImagePathText().Text(winrt::hstring{ std::wstring(L"File: ") + m_state.imagePath });
            PixelDimensionsText().Text(winrt::hstring{ dimensions });
            PixelFormatText().Text(winrt::hstring{ pixelFormat });
            StatusBarText().Text(winrt::hstring{ status });
        }
        else
        {
            ImagePathText().Text(L"File: none");
            PixelDimensionsText().Text(L"Dimensions: unknown");
            PixelFormatText().Text(L"Pixel format: unknown");
            StatusBarText().Text(m_state.preferHdr ? L"HDR preference is ON. Vulkan backend is currently a stub backend placeholder." : L"HDR preference is OFF. Vulkan backend is currently a stub backend placeholder.");
        }
    }
}
