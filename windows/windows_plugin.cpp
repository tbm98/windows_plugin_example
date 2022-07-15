#include "windows_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <ppltasks.h>

#include <memory>
#include <sstream>

#pragma once
#include <winrt/Windows.Services.Store.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Web.Syndication.h>
#include <iostream>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Syndication;
using namespace Windows::Services::Store;
using namespace Concurrency;

namespace windows_plugin {



    // The registrar for this plugin, for accessing the window.
    flutter::PluginRegistrarWindows* registrar_;

// static
void WindowsPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {

    registrar_ = registrar;

  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "windows_plugin",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<WindowsPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

WindowsPlugin::WindowsPlugin() {}

WindowsPlugin::~WindowsPlugin() {}

HWND GetRootWindow(flutter::FlutterView* view) {
    return ::GetAncestor(view->GetNativeWindow(), GA_ROOT);
}

void WindowsPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    auto hwnd = GetRootWindow(registrar_->GetView());
    printf("%p", hwnd);
    version_stream << hwnd;
    StoreContext storeContext = StoreContext::GetDefault();
    //storeContext.GetAppLicenseAsync();
    //concurrency::create_task(storeContext.GetAppLicenseAsync()).then([](StoreAppLicense license)
    //    {
    //        printf("%d", license.IsActive());
    //        
    //    }, task_continuation_context::get_current_winrt_context());
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
      
      
    }
    result->Success(flutter::EncodableValue(version_stream.str()));
  } else {
    result->NotImplemented();
  }
}

}  // namespace windows_plugin
