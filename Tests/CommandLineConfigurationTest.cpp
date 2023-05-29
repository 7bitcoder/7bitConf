#include "SevenBit/Conf/CommandLineConfiguration.hpp"
#include "SevenBit/Conf/Json.hpp"
#include <gtest/gtest.h>
#include <iostream>

class CommandLineConfigurationTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CommandLineConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CommandLineConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(CommandLineConfigurationTest, ShouldLoadConfFromArgs)
{
    const char *argv[] = {
        "progtam/path",
        "--string=test",
        "--list=string,string1,string2",
        "--object:inner:object=string",
    };
    auto provider = sb::cf::CommandLineConfigurationSource::create(4, const_cast<char **>(argv))->build();

    provider->load();

    sb::cf::JsonObject expected = {{"string", "test"},
                                   {"list", sb::cf::JsonArray{"string", "string1", "string2"}},
                                   {"object", {{"inner", {{"object", "string"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(CommandLineConfigurationTest, ShouldLoadEmptyConfFromArgs)
{
    const char *argv[] = {
        "progtam/path",
    };
    auto provider = sb::cf::CommandLineConfigurationSource::create(1, const_cast<char **>(argv))->build();

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}