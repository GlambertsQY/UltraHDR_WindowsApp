#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#define NOMINMAX
#include <windows.h>

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#if __has_include(<microsoft.ui.xaml.window.h>)
#include <microsoft.ui.xaml.window.h>
#else
struct __declspec(uuid("EECDBF0E-BAE9-4CB6-A68E-9598E1CB57BB")) IWindowNative : ::IUnknown
{
    virtual HRESULT __stdcall get_WindowHandle(HWND* hwnd) = 0;
};
#endif

#include <winrt/base.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Windows.Foundation.h>

#include "MainWindow.xaml.h"
