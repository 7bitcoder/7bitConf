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

TEST_F(ConfigurationTest, ExampleTest)
{
    auto ptr = sb::cf::ConfigurationBuilder{}.addAppSettings("dev").addJson({{"string", 112356}}).build();
    auto &conf = *ptr;

    EXPECT_EQ(conf["string"], 112356);
    EXPECT_EQ(conf["string"], 112356);
}
