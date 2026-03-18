#pragma once

#include <cstdint>
#include <string>
#include <vector>

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

    struct ImageData
    {
        std::uint32_t width{};
        std::uint32_t height{};
        std::uint32_t stride{};
        std::wstring pixelFormat;
        std::vector<std::uint8_t> pixels;
    };

    class IRenderBackend
    {
    public:
        virtual ~IRenderBackend() = default;

        virtual RenderBackendKind Kind() const noexcept = 0;
        virtual const wchar_t* Name() const noexcept = 0;
        virtual bool Initialize(HWND hwnd) = 0;
        virtual void Resize(std::uint32_t width, std::uint32_t height) = 0;
        virtual void UploadImage(ImageData image) = 0;
        virtual void ClearImage() = 0;
        virtual void Render(HDC dc, RECT const& bounds) = 0;
        virtual RenderCapabilities Capabilities() const = 0;
    };
}
