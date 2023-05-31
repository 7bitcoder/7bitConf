#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/AppSettingsConfiguration.hpp"

class AppSettingsConfiguration : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    AppSettingsConfiguration() {}

    void SetUp() override {}

    void TearDown() override {}

    ~AppSettingsConfiguration() {}

    static void TearDownTestSuite() {}
};

TEST_F(AppSettingsConfiguration, ShouldLoadAppSettings)
{
    auto provider = sb::cf::AppSettingsConfigurationSource::create()->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"number", 12345},
                                   {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                   {"string", "string"},
                                   {"object", {{"num", 134}, {"string", "string"}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(AppSettingsConfiguration, ShouldLoadDevAppSettings)
{
    auto provider = sb::cf::AppSettingsConfigurationSource::create("dev")->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"number", 12345},
                                   {"array", sb::cf::JsonArray{1}},
                                   {"string", "stringdev"},
                                   {"object", {{"num", 134}, {"string", "stringdev"}, {"inner", {{"num", 12345}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}