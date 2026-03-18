# Architecture

## Overview

The project uses a split UI/rendering architecture:

- `WinUI 3` provides desktop shell UI
- a native child `HWND` provides the rendering surface
- renderer backends are hidden behind `IRenderBackend`

This keeps Windows desktop integration straightforward while preserving room for backend evolution.

## Layers

### App shell

Files:

- `UltraHdrViewer/App.xaml*`
- `UltraHdrViewer/MainWindow.xaml*`

Responsibilities:

- application startup
- main window layout
- toolbar and command handling
- status and metadata presentation

### Viewport host

Files:

- `UltraHdrViewer/Host/ViewportHost.h`
- `UltraHdrViewer/Host/ViewportHost.cpp`

Responsibilities:

- create and destroy the child `HWND`
- keep it aligned with the XAML placeholder region
- forward size changes to the active render backend

### Renderer abstraction

Files:

- `UltraHdrViewer/Renderer/IRenderBackend.h`
- `UltraHdrViewer/Renderer/RenderBackendFactory.*`

Responsibilities:

- define common renderer lifecycle methods
- allow the shell to stay backend-agnostic

### Backends

Files:

- `UltraHdrViewer/Renderer/VulkanBackend.*`
- `UltraHdrViewer/Renderer/OpenGLBackend.*`

Current state:

- both are scaffolds
- `Vulkan` is the intended primary backend
- `OpenGL` is reserved for a future alternate implementation

## Why child HWND instead of SwapChainPanel

`SwapChainPanel` is best aligned with DXGI/DirectX swapchains. A child `HWND` works better for:

- Vulkan on Windows via `VK_KHR_win32_surface`
- future WGL/OpenGL support
- avoiding a renderer design that is locked to DXGI hosting rules

## Planned data flow

1. User opens an image from the WinUI shell
2. Image layer probes metadata immediately and then decodes SDR or Ultra HDR content
3. Decoded image data is uploaded to the active backend (currently a software preview path inside the Vulkan scaffold)
4. Backend renders into the child `HWND`
5. Inspector panel shows metadata and HDR state

## Near-term roadmap

1. Real Vulkan initialization
2. Replace the software preview with real Vulkan texture upload and visible presentation
3. `libultrahdr` integration
4. HDR capability detection
5. Tone mapping and presentation policy
