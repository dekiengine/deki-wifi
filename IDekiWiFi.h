#pragma once

#include "providers/IDekiModule.h"
#include <cstdint>
#include <cstddef>

/**
 * @brief One nearby WiFi access point reported by ScanAPs.
 */
struct DekiAP
{
    uint8_t  bssid[6]    = {0, 0, 0, 0, 0, 0};
    char     ssid[33]    = {0};   // null-terminated, up to 32 bytes per spec
    int8_t   rssi        = 0;     // dBm
    uint8_t  channel     = 0;
};

/**
 * @brief Abstract WiFi radio.
 *
 * Pure hardware abstraction: connect / disconnect / status / scan. No
 * credential storage, no provisioning UX, no auto-reconnect policy. Higher
 * layers (a boot helper, a captive-portal provisioning module, game code,
 * whoever) source credentials from wherever they please and call Connect()
 * explicitly.
 *
 * Implemented by whichever platform integration module is loaded at runtime.
 * The integration module registers its concrete driver with DekiWiFi::SetCurrent()
 * at module load. Single-active: there's one radio per chip, no multi-provider
 * registry needed for this category.
 */
class IDekiWiFi : public IDekiModule
{
public:
    const char* GetModuleCategory() const override { return "wifi"; }

    /// Connect to the given access point. Blocks up to timeoutMs.
    /// Returns true if associated and an IP was obtained within the timeout.
    /// On failure, the implementation logs the reason; subsequent IsConnected()
    /// returns false. Calling Connect again with new credentials replaces the
    /// previous attempt.
    virtual bool Connect(const char* ssid, const char* password, uint32_t timeoutMs) = 0;

    /// Disconnect from the current AP. Idempotent.
    virtual void Disconnect() = 0;

    /// True iff the station is currently associated and has an IP.
    virtual bool IsConnected() const = 0;

    /// Active scan of nearby APs. Blocks while scanning (typically ~2s).
    /// Writes up to maxCount entries into `out`. Returns the number written,
    /// or a negative value on driver error. Works while connected; the chip
    /// may briefly drop the active connection on some implementations.
    virtual int  ScanAPs(DekiAP* out, int maxCount) = 0;

    /// Tear down the WiFi stack. Calling Connect afterwards reinitialises.
    /// Normally only used for power saving or when intentionally going offline.
    virtual void Shutdown() = 0;
};
