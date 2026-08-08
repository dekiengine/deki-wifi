#pragma once

#ifdef _WIN32
    #ifdef DEKI_WIFI_EXPORTS
        #define DEKI_WIFI_API __declspec(dllexport)
    #else
        #define DEKI_WIFI_API __declspec(dllimport)
    #endif
#else
    #define DEKI_WIFI_API __attribute__((visibility("default")))
#endif
