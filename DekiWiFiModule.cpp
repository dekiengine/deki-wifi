#include "DekiWiFiPackage.h"
#include "DekiWiFi.h"
#include "interop/DekiPlugin.h"
#include "DekiLogSystem.h"

#ifdef DEKI_EDITOR
extern void DekiWiFi_RegisterComponents();
extern int  DekiWiFi_GetAutoComponentCount();
extern const DekiComponentMeta* DekiWiFi_GetAutoComponentMeta(int index);
#endif

static bool s_WiFiRegistered = false;

extern "C" {

DEKI_WIFI_API int DekiWiFi_EnsureRegistered(void)
{
#ifdef DEKI_EDITOR
    if (s_WiFiRegistered) return DekiWiFi_GetAutoComponentCount();
    s_WiFiRegistered = true;
    DekiWiFi_RegisterComponents();
    return DekiWiFi_GetAutoComponentCount();
#else
    return 0;
#endif
}

DEKI_PLUGIN_API const char* DekiPlugin_GetName(void)    { return "Deki WiFi Package"; }
DEKI_PLUGIN_API const char* DekiPlugin_GetVersion(void)
{
#ifdef DEKI_PACKAGE_VERSION
    return DEKI_PACKAGE_VERSION;
#else
    return "0.0.0-dev";
#endif
}
DEKI_PLUGIN_API const char* DekiPlugin_GetReflectionJson(void) { return "{}"; }

DEKI_PLUGIN_API int  DekiPlugin_Init(void)
{
    DEKI_LOG_INFO("[deki-wifi] DekiPlugin_Init");
    return 0;
}

DEKI_PLUGIN_API void DekiPlugin_Shutdown(void)
{
    // Null the active driver so a hot-reload of the integration package that
    // owns it doesn't leave a dangling pointer to its vtable.
    DekiWiFi::SetCurrent(nullptr);
    s_WiFiRegistered = false;
}

#ifdef DEKI_EDITOR
DEKI_PLUGIN_API int  DekiPlugin_GetComponentCount(void) { return DekiWiFi_GetAutoComponentCount(); }
DEKI_PLUGIN_API const DekiComponentMeta* DekiPlugin_GetComponentMeta(int index)
{
    return DekiWiFi_GetAutoComponentMeta(index);
}
#else
DEKI_PLUGIN_API int  DekiPlugin_GetComponentCount(void) { return 0; }
DEKI_PLUGIN_API const DekiComponentMeta* DekiPlugin_GetComponentMeta(int) { return nullptr; }
#endif

DEKI_PLUGIN_API void DekiPlugin_RegisterComponents(void)
{
#ifdef DEKI_EDITOR
    int n = DekiWiFi_EnsureRegistered();
    DEKI_LOG_INFO("[deki-wifi] DekiPlugin_RegisterComponents -> %d component(s)", n);
#endif
}

DEKI_PLUGIN_API int DekiPlugin_GetFeatureCount(void) { return 0; }
DEKI_PLUGIN_API const struct DekiPackageFeatureInfo* DekiPlugin_GetFeature(int) { return nullptr; }

// No providers registered here — this package owns only the interface and
// the SetCurrent/GetCurrent facade. Concrete drivers live in platform
// integration packages and call DekiWiFi::SetCurrent themselves.
DEKI_PLUGIN_API void DekiPlugin_RegisterPackages(void) {}

}  // extern "C"
