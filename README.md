# Deki WiFi

Docs: https://dekiengine.github.io/deki-wifi/ (components and properties, generated from the code)

Abstract WiFi peripheral interface for the Deki Engine. Defines `IDekiWiFi`: connect, disconnect, connection state, and access point scanning.

This package contains no radio driver of its own. A platform integration package (for example `deki-esp32-integration`) registers the concrete backend.

Part of [Deki Engine](https://github.com/dekiengine/deki-engine).

## Namespace

Types live in `DekiWifi`. Scene files store the qualified name, and so does code:

```cpp
using namespace DekiWifi;
obj->AddComponent<SomeComponent>();
```

Scenes saved before 0.16.0 used bare names and still load; saving writes the current one.

## Install

Package Manager in the Deki Editor, or `DekiEditor --packages-add deki-wifi <project>`.

## Dependencies

| Dependency | Type |
|---|---|
| ESP-IDF | External, only on ESP32 targets |

## License

Apache 2.0. See [LICENSE](LICENSE).
