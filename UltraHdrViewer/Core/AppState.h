#pragma once

#include <string>

#include "..\Renderer\IRenderBackend.h"

namespace UltraHdrViewer::Core
{
    struct AppState
    {
        UltraHdrViewer::Renderer::RenderBackendKind activeBackend{ UltraHdrViewer::Renderer::RenderBackendKind::Vulkan };
        bool preferHdr{ true };
        bool hasImage{ false };
        std::wstring sessionMessage{ L"" };
        std::wstring imagePath{ L"" };
        std::uint32_t pixelWidth{ 0 };
        std::uint32_t pixelHeight{ 0 };
        std::wstring pixelFormat{ L"Unknown" };
    };
}
