#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "SevenBit/Conf/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/KeyPerConfiguration.hpp"

class KeyPerConfigurationTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    KeyPerConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~KeyPerConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(KeyPerConfigurationTest, SimpleTest)
{
    auto provider = sb::cf::KeyPerConfigurationSource::create(
                        {
                            {"yeah", sb::cf::JsonFileConfigurationSource::create("appsettings.json")},
                        })
                        ->build();

    provider->load();

    sb::cf::JsonObject expected = {{"yeah",
                                    {{"number", 12345},
                                     {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                     {"string", "string"},
                                     {"object", {{"num", 134}, {"string", "string"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(KeyPerConfigurationTest, OverridedFileTest)
{

    auto provider = sb::cf::KeyPerConfigurationSource ::create(
                        {
                            {"yeah", sb::cf::JsonFileConfigurationSource::create("appsettings.json")},
                            {"dev", sb::cf::JsonFileConfigurationSource::create("appsettings.dev.json")},
                        })
                        ->build();

    provider->load();

    sb::cf::JsonObject expected = {{"yeah",
                                    {{"number", 12345},
                                     {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                     {"string", "string"},
                                     {"object", {{"num", 134}, {"string", "string"}}}}},
                                   {"dev",
                                    {{"array", sb::cf::JsonArray{1}},
                                     {"string", "stringdev"},
                                     {"object", {{"inner", {{"num", 12345}}}, {"string", "stringdev"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}