#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>

#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"

class ChainedConfigurationTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ChainedConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ChainedConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ChainedConfigurationTest, SimpleTest)
{
    auto provider = sb::cf::ChainedConfigurationSource{
        {
            std::make_unique<sb::cf::JsonFileConfigurationSource>("appsettings.json"),
        }}.build();

    provider->load();

    sb::cf::JsonObject expected = {{"number", 12345},
                                   {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                   {"string", "string"},
                                   {"object", {{"num", 134}, {"string", "string"}}}};
    EXPECT_EQ(provider->get(), expected);
}

TEST_F(ChainedConfigurationTest, OverridedFileTest)
{
    auto provider = sb::cf::ChainedConfigurationSource{
        {
            std::make_unique<sb::cf::JsonFileConfigurationSource>("appsettings.json"),
            std::make_unique<sb::cf::JsonFileConfigurationSource>("appsettings.dev.json"),
        }}.build();

    provider->load();

    sb::cf::JsonObject expected = {{"number", 12345},
                                   {"array", sb::cf::JsonArray{1}},
                                   {"string", "stringdev"},
                                   {"object", {{"num", 134}, {"string", "stringdev"}, {"inner", {{"num", 12345}}}}}};

    EXPECT_EQ(provider->get(), expected);
}