#include "pch.h"
#include "OpenGLBackend.h"

namespace UltraHdrViewer::Renderer
{
    RenderBackendKind OpenGLBackend::Kind() const noexcept
    {
        return RenderBackendKind::OpenGL;
    }

    const wchar_t* OpenGLBackend::Name() const noexcept
    {
        return L"OpenGL (reserved)";
    }

    bool OpenGLBackend::Initialize(HWND hwnd)
    {
        m_hwnd = hwnd;
        return m_hwnd != nullptr;
    }

    void OpenGLBackend::Resize(std::uint32_t, std::uint32_t)
    {
    }

    void OpenGLBackend::UploadImage(ImageData image)
    {
        m_image = std::move(image);

        if (m_hwnd)
        {
            InvalidateRect(m_hwnd, nullptr, FALSE);
        }
    }

    void OpenGLBackend::ClearImage()
    {
        m_image = {};

        if (m_hwnd)
        {
            InvalidateRect(m_hwnd, nullptr, FALSE);
        }
    }

    void OpenGLBackend::Render(HDC dc, RECT const& bounds)
    {
        HBRUSH fillBrush = CreateSolidBrush(RGB(7, 11, 16));
        FillRect(dc, &bounds, fillBrush);
        DeleteObject(fillBrush);

        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(216, 229, 237));

        std::wstring title = m_image.pixels.empty() ? L"OpenGL backend reserved" : L"OpenGL fallback not implemented";
        DrawTextW(dc, title.c_str(), static_cast<int>(title.size()), const_cast<RECT*>(&bounds), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    RenderCapabilities OpenGLBackend::Capabilities() const
    {
        return {
            .supportsHdrOutput = false,
            .supportsNativeUltraHdrSampling = false,
            .label = L"Reserved backend interface for future SDR/OpenGL support"
        };
    }
}
