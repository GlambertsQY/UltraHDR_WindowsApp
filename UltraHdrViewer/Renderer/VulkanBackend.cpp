#include "pch.h"
#include "VulkanBackend.h"

#include <algorithm>

namespace UltraHdrViewer::Renderer
{
    namespace
    {
        void FillBackground(HDC dc, RECT const& bounds)
        {
            HBRUSH fillBrush = CreateSolidBrush(RGB(7, 11, 16));
            FillRect(dc, &bounds, fillBrush);
            DeleteObject(fillBrush);
        }

        void DrawCenteredText(HDC dc, RECT bounds, wchar_t const* title, wchar_t const* subtitle)
        {
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(216, 229, 237));
            DrawTextW(dc, title, -1, &bounds, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            RECT footer = bounds;
            footer.top += 36;
            DrawTextW(dc, subtitle, -1, &footer, DT_CENTER | DT_TOP | DT_SINGLELINE);
        }
    }

    RenderBackendKind VulkanBackend::Kind() const noexcept
    {
        return RenderBackendKind::Vulkan;
    }

    const wchar_t* VulkanBackend::Name() const noexcept
    {
        return L"Vulkan (scaffold preview)";
    }

    bool VulkanBackend::Initialize(HWND hwnd)
    {
        m_hwnd = hwnd;
        return m_hwnd != nullptr;
    }

    void VulkanBackend::Resize(std::uint32_t width, std::uint32_t height)
    {
        m_width = width;
        m_height = height;
    }

    void VulkanBackend::UploadImage(ImageData image)
    {
        m_image = std::move(image);

        if (m_hwnd)
        {
            InvalidateRect(m_hwnd, nullptr, FALSE);
        }
    }

    void VulkanBackend::ClearImage()
    {
        m_image = {};

        if (m_hwnd)
        {
            InvalidateRect(m_hwnd, nullptr, FALSE);
        }
    }

    void VulkanBackend::Render(HDC dc, RECT const& bounds)
    {
        FillBackground(dc, bounds);

        if (m_image.pixels.empty() || m_image.width == 0 || m_image.height == 0)
        {
            DrawCenteredText(dc, bounds, L"Native Vulkan viewport host", Name());
            return;
        }

        const auto targetWidth = std::max(bounds.right - bounds.left, 1L);
        const auto targetHeight = std::max(bounds.bottom - bounds.top, 1L);

        const double scaleX = static_cast<double>(targetWidth) / static_cast<double>(m_image.width);
        const double scaleY = static_cast<double>(targetHeight) / static_cast<double>(m_image.height);
        const double scale = std::min(scaleX, scaleY);

        const auto drawWidth = std::max(1L, static_cast<LONG>(std::lround(static_cast<double>(m_image.width) * scale)));
        const auto drawHeight = std::max(1L, static_cast<LONG>(std::lround(static_cast<double>(m_image.height) * scale)));
        const auto drawLeft = bounds.left + (targetWidth - drawWidth) / 2;
        const auto drawTop = bounds.top + (targetHeight - drawHeight) / 2;

        BITMAPINFO bitmapInfo{};
        bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
        bitmapInfo.bmiHeader.biWidth = static_cast<LONG>(m_image.width);
        bitmapInfo.bmiHeader.biHeight = -static_cast<LONG>(m_image.height);
        bitmapInfo.bmiHeader.biPlanes = 1;
        bitmapInfo.bmiHeader.biBitCount = 32;
        bitmapInfo.bmiHeader.biCompression = BI_RGB;

        SetStretchBltMode(dc, HALFTONE);
        StretchDIBits(
            dc,
            drawLeft,
            drawTop,
            drawWidth,
            drawHeight,
            0,
            0,
            static_cast<int>(m_image.width),
            static_cast<int>(m_image.height),
            m_image.pixels.data(),
            &bitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY);
    }

    RenderCapabilities VulkanBackend::Capabilities() const
    {
        return {
            .supportsHdrOutput = false,
            .supportsNativeUltraHdrSampling = false,
            .label = L"Software preview today, Vulkan surface + swapchain initialization next"
        };
    }
}
