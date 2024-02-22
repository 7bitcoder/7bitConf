#include <gtest/gtest.h>
#include <iostream>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Sources/CommandLineConfiguration.hpp"

class CommandLineConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    CommandLineConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(CommandLineConfigurationTest, ShouldFailCreationDueToNullSource)
{
    std::vector<std::string_view> args;
    EXPECT_THROW(auto result = sb::cf::CommandLineConfigurationSource::create(args, nullptr),
                 sb::cf::NullPointerException);
}

TEST_F(CommandLineConfigurationTest, ShouldLoadConfFromArgs)
{
    const char *const argv[] = {"program/path",
                                "--string!string=test",
                                "--string_sep!string",
                                "sep_value",
                                "--list:0!string=string",
                                "double!double=1.22",
                                "true_bool!bool=-1",
                                "false_bool!bool=0",
                                "false_bool2!bool=false",
                                "true_bool2!bool=true",
                                "--list:1=string1",
                                "list:2!string=string2",
                                "--object:inner:object=string",
                                "--int_list:0!int=33",
                                "--int_list:1!int=22",
                                "int_list:2!int=11",
                                "--last!int"};
    int size = sizeof(argv) / sizeof(char *);
    auto provider =
        sb::cf::CommandLineConfigurationSource::create(size, argv, sb::cf::CommandLineParserBuilder{}.build())
            ->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"string", "test"},
                                   {"string_sep", "sep_value"},
                                   {"double", 1.22},
                                   {"false_bool", false},
                                   {"false_bool2", false},
                                   {"true_bool", true},
                                   {"true_bool2", true},
                                   {"last", 0},
                                   {"list", sb::cf::JsonArray{"string", "string1", "string2"}},
                                   {"int_list", sb::cf::JsonArray{33, 22, 11}},
                                   {"object", {{"inner", {{"object", "string"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(CommandLineConfigurationTest, ShouldLoadConfFromArgsVector)
{
    std::vector<std::string_view> args = {"--string!string=test",
                                          "--string_sep!string",
                                          "sep_value",
                                          "--list:0!string=string",
                                          "double!double=1.22",
                                          "true_bool!bool=-1",
                                          "false_bool!bool=0",
                                          "false_bool2!bool=false",
                                          "true_bool2!bool=true",
                                          "--list:1=string1",
                                          "list:2!string=string2",
                                          "--object:inner:object=string",
                                          "--int_list:0!int=33",
                                          "--int_list:1!int=22",
                                          "int_list:2!int=11",
                                          "--last!int"};
    auto provider =
        sb::cf::CommandLineConfigurationSource::create(args, sb::cf::CommandLineParserBuilder{}.build())->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"string", "test"},
                                   {"double", 1.22},
                                   {"string_sep", "sep_value"},
                                   {"false_bool", false},
                                   {"false_bool2", false},
                                   {"true_bool", true},
                                   {"true_bool2", true},
                                   {"last", 0},
                                   {"list", sb::cf::JsonArray{"string", "string1", "string2"}},
                                   {"int_list", sb::cf::JsonArray{33, 22, 11}},
                                   {"object", {{"inner", {{"object", "string"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(CommandLineConfigurationTest, ShouldLoadEmptyConfFromArgs)
{
    const char *argv[] = {
        "exec/path",
    };
    int size = sizeof(argv) / sizeof(char *);
    auto provider =
        sb::cf::CommandLineConfigurationSource::create(size, argv, sb::cf::CommandLineParserBuilder{}.build())
            ->build(mock);

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}
