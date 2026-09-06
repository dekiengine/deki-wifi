#include "DekiWiFi.h"

namespace {
    IDekiWiFi* s_Current = nullptr;
}

void DekiWiFi::SetCurrent(IDekiWiFi* driver)
{
    s_Current = driver;
}

IDekiWiFi* DekiWiFi::GetCurrent()
{
    return s_Current;
}
