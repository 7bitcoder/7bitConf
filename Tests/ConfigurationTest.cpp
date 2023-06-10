#include <gtest/gtest.h>
#include <iostream>
#include <string_view>

#include "SevenBit/Conf/ConfigurationBuilder.hpp"
#include "SevenBit/Conf/ObjectHolder.hpp"

class ConfigurationTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ConfigurationTest, ShouldLoadConfig)
{
    auto conf = sb::cf::ConfigurationBuilder{}
                    .addAppSettings("dev")
                    .addJson({{"string", 1}})
                    .addCommandLine({"--string=2", "Array:0!int=33"})
                    .AddInMemory("set:set", 44444)
                    .addKeyPerFile("Directory")
                    .build();

    sb::cf::JsonObject expected = {{"Array", sb::cf::JsonArray{33, 2, 3, 4, 5}},
                                   {"MySetting", "appsettings.dev.json Value"},
                                   {"ExtraSetting", "extra appsettings.dev.json Value"},
                                   {"Logging", {{"LogLevel", {{"Default", "Warning"}}}}},
                                   {"set", {{"set", 44444}}},
                                   {"string", "2"},
                                   {"settingOne",
                                    {{"number", 12345},
                                     {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                     {"string", "string"},
                                     {"object", {{"num", 134}, {"string", "string"}}}}},
                                   {"settingTwo",
                                    {{"array", sb::cf::JsonArray{1}},
                                     {"string", "stringdev"},
                                     {"object", {{"inner", {{"num", 12345}}}, {"string", "stringdev"}}}}}};

    EXPECT_EQ(conf->root(), expected);
}
