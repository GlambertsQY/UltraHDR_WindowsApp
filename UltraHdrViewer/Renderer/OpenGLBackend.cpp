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

    void OpenGLBackend::Render()
    {
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
