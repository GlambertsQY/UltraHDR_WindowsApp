#pragma once

#include <string>

#include <windows.h>

#include "..\Renderer\IRenderBackend.h"

namespace UltraHdrViewer::Host
{
    class ViewportHost
    {
    public:
        ViewportHost() = default;
        ~ViewportHost();

        bool Create(HWND parentHwnd, UltraHdrViewer::Renderer::IRenderBackend* backend);
        void Destroy();
        void SetBounds(RECT bounds);
        std::wstring Describe() const;

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
        LRESULT HandleMessage(UINT message, WPARAM wparam, LPARAM lparam);
        static const wchar_t* ClassName() noexcept;

        HWND m_parentHwnd{};
        HWND m_hwnd{};
        RECT m_bounds{};
        UltraHdrViewer::Renderer::IRenderBackend* m_backend{};
    };
}
