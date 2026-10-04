#pragma once

#include <deki/providers/IPackage.h>
#include <cstdint>
#include <cstddef>

namespace DekiWifi
{

/// One nearby WiFi access point, as ScanAPs reports it.
struct DekiAP
{
    uint8_t bssid[6] = { 0, 0, 0, 0, 0, 0 };
    char ssid[33] = { 0 };  // null-terminated, up to 32 bytes per spec
    int8_t rssi = 0;        // dBm
    uint8_t channel = 0;
};

/// A WiFi radio.
///
/// Hardware only: connect, disconnect, status, scan. It stores no
/// credentials, has no provisioning UI and no auto-reconnect policy. Higher
/// layers (a boot helper, a provisioning package, game code) get credentials
/// wherever they like and call Connect().
///
/// The platform integration package loaded at run time implements it and
/// registers its driver with DekiWiFi::SetCurrent() at package load. One
/// active driver: a chip has one radio.
class IDekiWiFi : public Deki::IPackage
{
public:
    const char* GetPackageCategory() const override { return "wifi"; }

    /// Connects to the access point, blocking up to timeoutMs. Returns true
    /// if it associated and got an IP within the timeout. On failure the
    /// implementation logs the reason and IsConnected() returns false.
    /// Calling Connect again with new credentials replaces the previous
    /// attempt.
    virtual bool Connect(const char* ssid, const char* password, uint32_t timeoutMs) = 0;

    /// Disconnects from the current AP. Safe to call when not connected.
    virtual void Disconnect() = 0;

    /// True when the station is associated and has an IP.
    virtual bool IsConnected() const = 0;

    /// Actively scans for nearby APs, blocking while it scans (usually ~2 s).
    /// Writes up to maxCount entries into `out` and returns how many, or a
    /// negative value on a driver error. Works while connected, though some
    /// chips briefly drop the connection.
    virtual int ScanAPs(DekiAP* out, int maxCount) = 0;

    /// Tears down the WiFi stack; a later Connect starts it again. For power
    /// saving or going offline on purpose.
    virtual void Shutdown() = 0;
};

}  // namespace DekiWifi
