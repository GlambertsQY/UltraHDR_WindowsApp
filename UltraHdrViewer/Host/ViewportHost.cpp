#include "pch.h"
#include "ViewportHost.h"

namespace UltraHdrViewer::Host
{
    ViewportHost::~ViewportHost()
    {
        Destroy();
    }

    bool ViewportHost::Create(HWND parentHwnd, UltraHdrViewer::Renderer::IRenderBackend* backend)
    {
        if (m_hwnd)
        {
            return true;
        }

        static bool registered = false;
        if (!registered)
        {
            WNDCLASSEXW windowClass{};
            windowClass.cbSize = sizeof(windowClass);
            windowClass.style = CS_HREDRAW | CS_VREDRAW;
            windowClass.lpfnWndProc = &ViewportHost::WindowProc;
            windowClass.hInstance = GetModuleHandleW(nullptr);
            windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
            windowClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
            windowClass.lpszClassName = ClassName();

            RegisterClassExW(&windowClass);
            registered = true;
        }

        m_parentHwnd = parentHwnd;
        m_backend = backend;

        m_hwnd = CreateWindowExW(
            0,
            ClassName(),
            L"UltraHDRViewport",
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            0,
            0,
            1,
            1,
            m_parentHwnd,
            nullptr,
            GetModuleHandleW(nullptr),
            this);

        if (!m_hwnd)
        {
            return false;
        }

        if (m_backend)
        {
            m_backend->Initialize(m_hwnd);
        }

        return true;
    }

    void ViewportHost::Destroy()
    {
        if (m_hwnd)
        {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }
    }

    void ViewportHost::SetBounds(RECT bounds)
    {
        m_bounds = bounds;

        if (!m_hwnd)
        {
            return;
        }

        const auto width = std::max<LONG>(bounds.right - bounds.left, 1);
        const auto height = std::max<LONG>(bounds.bottom - bounds.top, 1);

        SetWindowPos(m_hwnd, nullptr, bounds.left, bounds.top, width, height, SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);

        if (m_backend)
        {
            m_backend->Resize(static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height));
        }
    }

    void ViewportHost::Invalidate()
    {
        if (m_hwnd)
        {
            InvalidateRect(m_hwnd, nullptr, FALSE);
        }
    }

    std::wstring ViewportHost::Describe() const
    {
        if (!m_hwnd)
        {
            return L"Native viewport not created";
        }

        const auto width = std::max<LONG>(m_bounds.right - m_bounds.left, 1);
        const auto height = std::max<LONG>(m_bounds.bottom - m_bounds.top, 1);

        std::wstring description = L"Native child HWND online";
        description += L" - ";
        description += std::to_wstring(width);
        description += L"x";
        description += std::to_wstring(height);

        if (m_backend)
        {
            description += L" - backend: ";
            description += m_backend->Name();
        }

        return description;
    }

    LRESULT CALLBACK ViewportHost::WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
    {
        if (message == WM_NCCREATE)
        {
            auto* createStruct = reinterpret_cast<CREATESTRUCTW*>(lparam);
            auto* self = static_cast<ViewportHost*>(createStruct->lpCreateParams);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
            self->m_hwnd = hwnd;
        }

        auto* self = reinterpret_cast<ViewportHost*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        if (!self)
        {
            return DefWindowProcW(hwnd, message, wparam, lparam);
        }

        return self->HandleMessage(message, wparam, lparam);
    }

    LRESULT ViewportHost::HandleMessage(UINT message, WPARAM wparam, LPARAM lparam)
    {
        switch (message)
        {
        case WM_SIZE:
            if (m_backend)
            {
                m_backend->Resize(static_cast<std::uint32_t>(LOWORD(lparam)), static_cast<std::uint32_t>(HIWORD(lparam)));
            }
            InvalidateRect(m_hwnd, nullptr, FALSE);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT paint{};
            auto dc = BeginPaint(m_hwnd, &paint);

            RECT client{};
            GetClientRect(m_hwnd, &client);

            if (m_backend)
            {
                m_backend->Render(dc, client);
            }
            else
            {
                HBRUSH fillBrush = CreateSolidBrush(RGB(7, 11, 16));
                FillRect(dc, &client, fillBrush);
                DeleteObject(fillBrush);
            }

            EndPaint(m_hwnd, &paint);
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;
        }

        return DefWindowProcW(m_hwnd, message, wparam, lparam);
    }

    const wchar_t* ViewportHost::ClassName() noexcept
    {
        return L"UltraHdrViewer.ViewportHost";
    }
}
