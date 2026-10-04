#pragma once

#include "IDekiWiFi.h"
#include "DekiWiFiPackage.h"

namespace DekiWifi
{

/// Holds the one active WiFi driver.
///
/// Like DekiHttp and DekiGps::DekiGPS: a platform integration package sets
/// its driver with SetCurrent(), and consumers (location providers,
/// provisioning, game code) reach it through GetCurrent().
///
/// One active driver, not a multi-provider registry: a chip has one WiFi
/// radio, and swapping drivers at run time is not a real use. A board with a
/// second radio (an SPI WiFi co-processor) could move this category to the
/// multi-provider pattern without changing consumers' call sites.
class DEKI_WIFI_API DekiWiFi
{
public:
    static void SetCurrent(IDekiWiFi* driver);
    static IDekiWiFi* GetCurrent();
};

}  // namespace DekiWifi
