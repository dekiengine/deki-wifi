#pragma once

#include "IDekiWiFi.h"
#include "DekiWiFiPackage.h"

/**
 * @brief Active-driver registry for WiFi (single-instance).
 *
 * Mirrors the DekiHttp / DekiGPS pattern: a platform integration package
 * registers its driver via SetCurrent() at DekiPlugin_RegisterPackages time.
 * Consumers (location providers, future provisioning packages, game code)
 * reach the active driver via GetCurrent().
 *
 * Single-active rather than the multi-provider registry pattern: there is
 * one WiFi radio per chip, swapping out the driver at runtime is not a
 * realistic use case. If a board ever ships a secondary radio (SPI WiFi
 * co-processor), we can switch this category to the multi-provider pattern
 * without changing consumers' call sites.
 */
class DEKI_WIFI_API DekiWiFi
{
public:
    static void       SetCurrent(IDekiWiFi* driver);
    static IDekiWiFi* GetCurrent();
};
