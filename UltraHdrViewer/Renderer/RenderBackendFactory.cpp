#include "pch.h"
#include "RenderBackendFactory.h"

#include "OpenGLBackend.h"
#include "VulkanBackend.h"

namespace UltraHdrViewer::Renderer
{
    std::unique_ptr<IRenderBackend> CreateRenderBackend(RenderBackendKind kind)
    {
        switch (kind)
        {
        case RenderBackendKind::OpenGL:
            return std::make_unique<OpenGLBackend>();
        case RenderBackendKind::Vulkan:
        default:
            return std::make_unique<VulkanBackend>();
        }
    }
}
