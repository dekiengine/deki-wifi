// Package entry point of deki-wifi.
#include "DekiWiFiPackage.h"
#include "DekiWiFi.h"
#include <deki/interop/Plugin.h>
#include <deki/LogSystem.h>

extern void DekiWiFiRegisterComponents();
extern int DekiWiFiGetAutoComponentCount();
extern const Deki::ComponentMeta* DekiWiFiGetAutoComponentMeta(int index);

namespace DekiWifi
{

#ifdef DEKI_EDITOR
#endif

static bool s_WiFiRegistered = false;

}  // namespace DekiWifi
// The exports below are C symbols at global scope; the package's own
// registration helpers and statics live in its namespace.
using namespace DekiWifi;

extern "C"
{
    DEKI_WIFI_API int DekiWiFiEnsureRegistered(void)
    {
#ifdef DEKI_EDITOR
        if (s_WiFiRegistered)
        {
            return ::DekiWiFiGetAutoComponentCount();
        }
        s_WiFiRegistered = true;
        ::DekiWiFiRegisterComponents();
        return ::DekiWiFiGetAutoComponentCount();
#else
        return 0;
#endif
    }

    DEKI_PLUGIN_API const char* DekiPluginGetName(void)
    {
        return "Deki WiFi Package";
    }
    DEKI_PLUGIN_API const char* DekiPluginGetVersion(void)
    {
#ifdef DEKI_PACKAGE_VERSION
        return DEKI_PACKAGE_VERSION;
#else
        return "0.0.0-dev";
#endif
    }

    DEKI_PLUGIN_API int DekiPluginInit(void)
    {
        return 0;
    }

    DEKI_PLUGIN_API void DekiPluginShutdown(void)
    {
        // Clear the active driver, so a hot reload of the integration package
        // that owns it leaves no dangling pointer to its vtable.
        DekiWiFi::SetCurrent(nullptr);
        s_WiFiRegistered = false;
    }

#ifdef DEKI_EDITOR
    DEKI_PLUGIN_API int DekiPluginGetComponentCount(void)
    {
        return ::DekiWiFiGetAutoComponentCount();
    }
    DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPluginGetComponentMeta(int index)
    {
        return ::DekiWiFiGetAutoComponentMeta(index);
    }
#else
    DEKI_PLUGIN_API int DekiPluginGetComponentCount(void)
    {
        return 0;
    }
    DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPluginGetComponentMeta(int)
    {
        return nullptr;
    }
#endif

    DEKI_PLUGIN_API void DekiPluginRegisterComponents(void)
    {
#ifdef DEKI_EDITOR
        DekiWiFiEnsureRegistered();
#endif
    }

    // This package holds only the IDekiWiFi interface and the
    // SetCurrent/GetCurrent facade; it registers no driver of its own. The
    // drivers live in the platform integration packages and call
    // DekiWiFi::SetCurrent themselves.

}  // extern "C"
