#pragma once

#include <cstdint>
#include <string>

#include <windows.h>

namespace UltraHdrViewer::Renderer
{
    enum class RenderBackendKind
    {
        Vulkan,
        OpenGL,
    };

    struct RenderCapabilities
    {
        bool supportsHdrOutput{ false };
        bool supportsNativeUltraHdrSampling{ false };
        std::wstring label;
    };

    class IRenderBackend
    {
    public:
        virtual ~IRenderBackend() = default;

        virtual RenderBackendKind Kind() const noexcept = 0;
        virtual const wchar_t* Name() const noexcept = 0;
        virtual bool Initialize(HWND hwnd) = 0;
        virtual void Resize(std::uint32_t width, std::uint32_t height) = 0;
        virtual void Render() = 0;
        virtual RenderCapabilities Capabilities() const = 0;
    };
}
