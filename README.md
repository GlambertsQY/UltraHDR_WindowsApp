# UltraHdrViewer

`UltraHdrViewer` is a Windows desktop Ultra HDR photo viewer scaffold targeting `WinUI 3 + C++/WinRT + Vulkan`, with an `OpenGL` backend reserved behind a renderer abstraction. The current version focuses on establishing a solid VS2022-native foundation before image decoding and HDR presentation are added.

## Goals

- Build a Windows-native Ultra HDR viewer around `libultrahdr`
- Use `WinUI 3` for desktop shell, commands, and metadata panels
- Use a native child `HWND` viewport so `Vulkan` works cleanly today and `OpenGL` can be added later
- Start from an `unpackaged` development workflow in Visual Studio 2022

## Current status

- VS2022 solution and project build successfully in `Debug|x64`
- WinUI 3 desktop shell is scaffolded and launches as an unpackaged app
- Main window layout includes command bar, viewport area, inspector panel, and status bar
- Native child `HWND` viewport host is created and synchronized with XAML layout
- Renderer abstraction is in place with `Vulkan` and `OpenGL` placeholder backends
- Build artifacts such as generated XAML glue, PRI, and bootstrap runtime files are emitted correctly

## Project layout

- `UltraHdrViewer.sln` - Visual Studio 2022 solution entry point
- `UltraHdrViewer/UltraHdrViewer.vcxproj` - WinUI 3 C++/WinRT desktop project
- `UltraHdrViewer/App.xaml*` - application bootstrap and launch flow
- `UltraHdrViewer/MainWindow.xaml*` - shell UI and viewport integration
- `UltraHdrViewer/Host/` - native child `HWND` viewport hosting
- `UltraHdrViewer/Renderer/` - backend abstraction, Vulkan stub, OpenGL reserve stub
- `UltraHdrViewer/Core/` - app state models

## Prerequisites

- Visual Studio 2022
- `WinUI application development` workload
- `C++ WinUI app development tools`
- Windows 11 SDK
- Windows App SDK / WinUI 3 C++ tooling

## Build

1. Open `UltraHdrViewer.sln` in Visual Studio 2022
2. Select `Debug | x64`
3. Build and run the `UltraHdrViewer` project

The project is currently configured for unpackaged development. The build produces the app binary and required Windows App SDK bootstrap/runtime side files in `bin/x64/Debug`.

## Architecture summary

- `WinUI shell` owns commands, layout, and metadata surfaces
- `ViewportHost` owns a native child `HWND` placed inside the WinUI layout
- `IRenderBackend` isolates rendering from the shell
- `VulkanBackend` is the planned primary renderer
- `OpenGLBackend` is reserved for a later fallback/alternate backend

More detail is documented in `docs/ARCHITECTURE.md`.

## Current limitations

- No real Vulkan swapchain yet; the backend is still a stub
- No file picker or image decoding yet
- No `libultrahdr` integration yet
- No HDR capability probing or tone mapping path yet

## Next steps

1. Replace the Vulkan stub with real instance, surface, and swapchain setup
2. Add file open flow and standard image loading
3. Integrate `libultrahdr`
4. Add HDR display capability detection and SDR fallback logic

## Notes

- The viewport uses a child `HWND` by design to avoid forcing Vulkan through DXGI-centric XAML paths
- Because of WinUI/XAML airspace rules, viewport overlays should eventually be rendered either outside the viewport or inside the graphics backend itself
