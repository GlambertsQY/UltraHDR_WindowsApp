#pragma once

#include <string>

#include "..\Renderer\IRenderBackend.h"

namespace UltraHdrViewer::Core
{
    struct AppState
    {
        UltraHdrViewer::Renderer::RenderBackendKind activeBackend{ UltraHdrViewer::Renderer::RenderBackendKind::Vulkan };
        bool preferHdr{ true };
        std::wstring sessionMessage{ L"" };
    };
}
