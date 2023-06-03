#include "SevenBit/Conf/CommandLineConfiguration.hpp"
#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Json.hpp"
#include <gtest/gtest.h>
#include <iostream>

class CommandLineConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    CommandLineConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CommandLineConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(CommandLineConfigurationTest, ShouldLoadConfFromArgs)
{
    const char *const argv[] = {
        "progtam/path",     "--string=test",    "--list:0=string",
        "--list:1=string1", "--list:2=string2", "--object:inner:object=string",
    };
    int size = sizeof(argv) / sizeof(char *);
    auto provider = sb::cf::CommandLineConfigurationSource::create(size, argv)->build(mock);

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
    int size = sizeof(argv) / sizeof(char *);
    auto provider = sb::cf::CommandLineConfigurationSource::create(size, argv)->build(mock);

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}