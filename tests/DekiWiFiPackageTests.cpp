// deki-wifi: the WiFi driver a platform package makes current, and the package's plugin entry points, which
// register its components with the editor once however often it asks and let
// a reloaded package register them again.
//
// The registration functions reflection codegen generates are stood in for
// here, so the entry file is tested as it is.

#include <gtest/gtest.h>

#include "DekiWiFi.h"

#include <deki/PackageConfig.h>
#include <deki/interop/Plugin.h>

#include <cstring>

using namespace DekiWifi;

namespace
{
int g_Registered = 0;
}

// Stand-ins for the generated registration functions the entry file calls.
void DekiWiFiRegisterComponents()
{
    ++g_Registered;
}
int DekiWiFiGetAutoComponentCount()
{
    return 3;
}
const Deki::ComponentMeta* DekiWiFiGetAutoComponentMeta(int)
{
    return nullptr;
}

// The entry points, from the package's entry file.
extern "C"
{
    int DekiWiFiEnsureRegistered(void);
    const char* DekiPluginGetName(void);
    const char* DekiPluginGetVersion(void);
    void DekiPluginShutdown(void);
    int DekiPluginGetComponentCount(void);
    void DekiPluginRegisterComponents(void);
}

namespace
{

class PackageTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        DekiPluginShutdown();
        g_Registered = 0;
    }
};

class Fake : public IDekiWiFi
{
public:
    const char* GetPackageId() const override { return "wifi"; }
    const char* GetPackageName() const override { return "Fake"; }
    void Configure(const Deki::PackageConfig&) override {}
    bool Initialize() override { return true; }
    void Shutdown() override {}
    void Update(float) override {}
    Deki::PackageState GetState() const override { return Deki::PackageState::Running; }
    const char* GetLastError() const override { return ""; }
    bool Connect(const char*, const char*, uint32_t) override { return false; }
    void Disconnect() override {}
    bool IsConnected() const override { return false; }
    int ScanAPs(DekiAP*, int) override { return 0; }
};

}  // namespace

TEST_F(PackageTest, TheCurrentDriverIsWhatWasSet)
{
    Fake driver;
    EXPECT_EQ(DekiWiFi::GetCurrent(), nullptr);
    DekiWiFi::SetCurrent(&driver);
    EXPECT_EQ(DekiWiFi::GetCurrent(), &driver);
    DekiWiFi::SetCurrent(nullptr);
    EXPECT_EQ(DekiWiFi::GetCurrent(), nullptr);
}

TEST_F(PackageTest, ShutdownForgetsTheCurrentDriver)
{
    // The driver lives in the platform package's DLL; kept past shutdown,
    // the editor would call into unloaded code after a reload.
    Fake driver;
    DekiWiFi::SetCurrent(&driver);
    DekiPluginShutdown();
    EXPECT_EQ(DekiWiFi::GetCurrent(), nullptr);
}

TEST_F(PackageTest, ComponentsAreRegisteredOnce)
{
    EXPECT_EQ(DekiWiFiEnsureRegistered(), 3);
    EXPECT_EQ(DekiWiFiEnsureRegistered(), 3);
    DekiPluginRegisterComponents();
    EXPECT_EQ(g_Registered, 1);
    EXPECT_EQ(DekiPluginGetComponentCount(), 3);
}

TEST_F(PackageTest, AReloadedPackageRegistersAgain)
{
    DekiWiFiEnsureRegistered();
    DekiPluginShutdown();
    DekiWiFiEnsureRegistered();
    EXPECT_EQ(g_Registered, 2);
}

TEST_F(PackageTest, ThePluginNamesItselfAndItsVersion)
{
    EXPECT_GT(std::strlen(DekiPluginGetName()), 0u);
    EXPECT_STREQ(DekiPluginGetVersion(), "tests");  // DEKI_PACKAGE_VERSION, as the build sets it
}
