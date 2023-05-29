#include <gtest/gtest.h>
#include <iostream>

#include "SevenBit/Conf/ConfigurationBuilder.hpp"

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
                    .addCommandLine({"--string=2", "array=3,2,1"})
                    .addKeyPerFile("Directory")
                    .build();

    sb::cf::JsonObject expected = {{"number", 12345},
                                   {"array", sb::cf::JsonArray{"3", "2", "1"}},
                                   {"string", "2"},
                                   {"object", {{"num", 134}, {"string", "stringdev"}, {"inner", {{"num", 12345}}}}},
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
