#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "SevenBit/Conf/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/KeyPerConfiguration.hpp"
#include "SevenBit/Conf/KeyPerFileConfiguration.hpp"

class KeyPerFileConfigurationTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    KeyPerFileConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~KeyPerFileConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(KeyPerFileConfigurationTest, NonExistingTest)
{
    auto provider = sb::cf::KeyPerFileConfigurationSource{"nonexisting"}.build();

    provider->load();

    sb::cf::JsonObject expected = {{"settingOne",
                                    {{"number", 12345},
                                     {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                     {"string", "string"},
                                     {"object", {{"num", 134}, {"string", "string"}}}}},
                                   {"settingTwo",
                                    {{"array", sb::cf::JsonArray{1}},
                                     {"string", "stringdev"},
                                     {"object", {{"inner", {{"num", 12345}}}, {"string", "stringdev"}}}}}};

    EXPECT_EQ(provider->get(), expected);
}

TEST_F(KeyPerFileConfigurationTest, SimpleTest)
{
    auto provider = sb::cf::KeyPerFileConfigurationSource{"Directory"}.build();

    provider->load();

    sb::cf::JsonObject expected = {{"settingOne",
                                    {{"number", 12345},
                                     {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                     {"string", "string"},
                                     {"object", {{"num", 134}, {"string", "string"}}}}},
                                   {"settingTwo",
                                    {{"array", sb::cf::JsonArray{1}},
                                     {"string", "stringdev"},
                                     {"object", {{"inner", {{"num", 12345}}}, {"string", "stringdev"}}}}}};

    EXPECT_EQ(provider->get(), expected);
}