#include <gtest/gtest.h>
#include <string>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Sources/AppSettingsConfiguration.hpp"

class AppSettingsConfiguration : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    AppSettingsConfiguration() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(AppSettingsConfiguration, ShouldLoadAppSettings)
{
    const auto provider = sb::cf::AppSettingsConfigurationSource::create()->build(mock);

    provider->load();

    const sb::cf::JsonObject expected = {{"Array", sb::cf::JsonArray{1, 2, 3, 4, 5}},
                                         {"MySetting", "appsettings.json Value"},
                                         {"Logging", {{"LogLevel", {{"Default", "Information"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(AppSettingsConfiguration, ShouldLoadDevAppSettings)
{
    const auto provider = sb::cf::AppSettingsConfigurationSource::create("dev")->build(mock);

    provider->load();

    const sb::cf::JsonObject expected = {{"Array", sb::cf::JsonArray{11, 2, 3, 4, 5}},
                                         {"MySetting", "appsettings.dev.json Value"},
                                         {"ExtraSetting", "extra appsettings.dev.json Value"},
                                         {"Logging", {{"LogLevel", {{"Default", "Warning"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}
