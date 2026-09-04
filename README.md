# Deki WiFi

Abstract WiFi peripheral interface for the Deki Engine. Defines `IDekiWiFi`: connect, disconnect, connection state, and access point scanning.

This package contains no radio driver of its own. A platform integration package (for example `deki-esp32-integration`) registers the concrete backend.

Part of the [Deki Engine](https://github.com/dekiengine/deki-engine) package ecosystem.

## Installation

Install via the Package Manager inside the Deki Editor.

## Dependencies

| Dependency | Type |
|---|---|
| ESP-IDF | External, only on ESP32 targets |

## License

Licensed under the Apache License, Version 2.0. See [LICENSE](LICENSE) for details.
