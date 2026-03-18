#include "pch.h"
#include "VulkanBackend.h"

namespace UltraHdrViewer::Renderer
{
    RenderBackendKind VulkanBackend::Kind() const noexcept
    {
        return RenderBackendKind::Vulkan;
    }

    const wchar_t* VulkanBackend::Name() const noexcept
    {
        return L"Vulkan (stub)";
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

    void VulkanBackend::Render()
    {
    }

    RenderCapabilities VulkanBackend::Capabilities() const
    {
        return {
            .supportsHdrOutput = false,
            .supportsNativeUltraHdrSampling = false,
            .label = L"Reserved for Vulkan surface + swapchain initialization"
        };
    }
}
