#pragma once

#include "pch.h"

#pragma warning(push)
#pragma warning(disable: 4100)

namespace winrt::UltraHdrViewer::implementation
{
    using Application = ::winrt::Microsoft::UI::Xaml::Application;
    using ComponentResourceLocation = ::winrt::Microsoft::UI::Xaml::Controls::Primitives::ComponentResourceLocation;
    using DependencyObject = ::winrt::Microsoft::UI::Xaml::DependencyObject;
    using IComponentConnector = ::winrt::Microsoft::UI::Xaml::Markup::IComponentConnector;

    template <typename D, typename... I>
    void MainWindowT<D, I...>::InitializeComponent()
    {
        if (!_contentLoaded)
        {
            _contentLoaded = true;
            ::winrt::Windows::Foundation::Uri resourceLocator{ L"ms-appx:///MainWindow.xaml" };
            ::winrt::Microsoft::UI::Xaml::Application::LoadComponent(*this, resourceLocator, ComponentResourceLocation::Application);
        }
    }

    template <typename D, typename... I>
    void MainWindowT<D, I...>::Connect(int32_t connectionId, IInspectable const& target)
    {
        switch (connectionId)
        {
        case 2:
            ContentRoot(target.as<::winrt::Microsoft::UI::Xaml::Controls::Grid>());
            break;
        case 3:
            StatusBarText(target.as<::winrt::Microsoft::UI::Xaml::Controls::TextBlock>());
            break;
        case 4:
            SessionStatusText(target.as<::winrt::Microsoft::UI::Xaml::Controls::TextBlock>());
            break;
        case 5:
            DisplayStatusText(target.as<::winrt::Microsoft::UI::Xaml::Controls::TextBlock>());
            break;
        case 6:
            ViewportPlaceholder(target.as<::winrt::Microsoft::UI::Xaml::Controls::Border>());
            break;
        case 7:
            ViewportStatusText(target.as<::winrt::Microsoft::UI::Xaml::Controls::TextBlock>());
            break;
        case 8:
        {
            auto targetElement = target.as<::winrt::Microsoft::UI::Xaml::Controls::Button>();
            auto weakThis = ::winrt::make_weak<class_type>(*this);
            targetElement.Click([weakThis](::winrt::Windows::Foundation::IInspectable const& p0, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& p1)
            {
                if (auto t = weakThis.get())
                {
                    ::winrt::get_self<D>(t)->OnOpenClicked(p0, p1);
                }
            });
            break;
        }
        case 9:
        {
            auto targetElement = target.as<::winrt::Microsoft::UI::Xaml::Controls::Button>();
            auto weakThis = ::winrt::make_weak<class_type>(*this);
            targetElement.Click([weakThis](::winrt::Windows::Foundation::IInspectable const& p0, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& p1)
            {
                if (auto t = weakThis.get())
                {
                    ::winrt::get_self<D>(t)->OnFitClicked(p0, p1);
                }
            });
            break;
        }
        case 10:
        {
            auto targetElement = target.as<::winrt::Microsoft::UI::Xaml::Controls::Button>();
            auto weakThis = ::winrt::make_weak<class_type>(*this);
            targetElement.Click([weakThis](::winrt::Windows::Foundation::IInspectable const& p0, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& p1)
            {
                if (auto t = weakThis.get())
                {
                    ::winrt::get_self<D>(t)->OnActualSizeClicked(p0, p1);
                }
            });
            break;
        }
        case 11:
        {
            auto targetElement = target.as<::winrt::Microsoft::UI::Xaml::Controls::Button>();
            auto weakThis = ::winrt::make_weak<class_type>(*this);
            targetElement.Click([weakThis](::winrt::Windows::Foundation::IInspectable const& p0, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& p1)
            {
                if (auto t = weakThis.get())
                {
                    ::winrt::get_self<D>(t)->OnHdrToggleClicked(p0, p1);
                }
            });
            break;
        }
        case 12:
            BackendStatusText(target.as<::winrt::Microsoft::UI::Xaml::Controls::TextBlock>());
            break;
        }

        _contentLoaded = true;
    }

    template <typename D, typename... I>
    void MainWindowT<D, I...>::DisconnectUnloadedObject(int32_t)
    {
        throw ::winrt::hresult_invalid_argument{ L"No unloadable objects to disconnect." };
    }

    template <typename D, typename... I>
    void MainWindowT<D, I...>::UnloadObject(DependencyObject const&)
    {
        throw ::winrt::hresult_invalid_argument{ L"No unloadable objects." };
    }

    template <typename D, typename... I>
    IComponentConnector MainWindowT<D, I...>::GetBindingConnector(int32_t, IInspectable const&)
    {
        return nullptr;
    }

    template struct MainWindowT<struct MainWindow>;
}

#pragma warning(pop)
