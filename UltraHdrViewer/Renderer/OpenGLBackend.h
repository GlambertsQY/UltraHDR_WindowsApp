#pragma once

#include "IRenderBackend.h"

namespace UltraHdrViewer::Renderer
{
    class OpenGLBackend final : public IRenderBackend
    {
    public:
        RenderBackendKind Kind() const noexcept override;
        const wchar_t* Name() const noexcept override;
        bool Initialize(HWND hwnd) override;
        void Resize(std::uint32_t width, std::uint32_t height) override;
        void UploadImage(ImageData image) override;
        void ClearImage() override;
        void Render(HDC dc, RECT const& bounds) override;
        RenderCapabilities Capabilities() const override;

    private:
        HWND m_hwnd{};
        ImageData m_image;
    };
}
