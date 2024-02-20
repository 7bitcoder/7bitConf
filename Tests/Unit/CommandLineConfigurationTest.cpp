// #include <gtest/gtest.h>
// #include <iostream>
//
// #include "Mocks/ConfigurationBuilderMock.hpp"
// #include "SevenBit/Conf/Exceptions.hpp"
// #include "SevenBit/Conf/Sources/CommandLineConfiguration.hpp"
//
// class CommandLineConfigurationTest : public testing::Test
// {
//   protected:
//     ConfigurationBuilderMock mock;
//
//     static void TearUpTestSuite() {}
//
//     CommandLineConfigurationTest() {}
//
//     void SetUp() override {}
//
//     void TearDown() override {}
//
//     static void TearDownTestSuite() {}
// };
//
// TEST_F(CommandLineConfigurationTest, ShouldFailCreationDueToNullSource)
// {
//     std::vector<std::string_view> args;
//     EXPECT_THROW(auto result = sb::cf::CommandLineConfigurationSource::create(args, nullptr),
//                  sb::cf::NullPointerException);
// }
//
// TEST_F(CommandLineConfigurationTest, ShouldLoadConfFromArgs)
// {
//     const char *const argv[] = {
//         "program/path",          "--string___string=test", "--list:0!string=string",        "double!double=1.22",
//         "true_bool!bool=-1",     "false_bool!bool=0",      "false_bool2!bool=false",        "true_bool2!bool=true",
//         "--list:1=string1",      "list__2!string=string2", "--object__inner:object=string", "--int_list:0___int=33",
//         "--int_list:1___int=22", "int_list__2!int=11",
//     };
//     int size = sizeof(argv) / sizeof(char *);
//     auto provider = sb::cf::CommandLineConfigurationSource::create(size, argv)->build(mock);
//
//     provider->load();
//
//     sb::cf::JsonObject expected = {{"string", "test"},
//                                    {"double", 1.22},
//                                    {"false_bool", false},
//                                    {"false_bool2", false},
//                                    {"true_bool", true},
//                                    {"true_bool2", true},
//                                    {"list", sb::cf::JsonArray{"string", "string1", "string2"}},
//                                    {"int_list", sb::cf::JsonArray{33, 22, 11}},
//                                    {"object", {{"inner", {{"object", "string"}}}}}};
//
//     EXPECT_EQ(provider->getConfiguration(), expected);
// }
//
// TEST_F(CommandLineConfigurationTest, ShouldLoadConfFromArgsVector)
// {
//     std::vector<std::string_view> args = {
//         "--string___string=test", "--list:0!string=string",        "double!double=1.22",    "true_bool!bool=-1",
//         "false_bool!bool=0",      "false_bool2!bool=false",        "true_bool2!bool=true",  "--list:1=string1",
//         "list__2!string=string2", "--object__inner:object=string", "--int_list:0___int=33", "--int_list:1___int=22",
//         "int_list__2!int=11",
//     };
//     auto provider = sb::cf::CommandLineConfigurationSource::create(args)->build(mock);
//
//     provider->load();
//
//     sb::cf::JsonObject expected = {{"string", "test"},
//                                    {"double", 1.22},
//                                    {"false_bool", false},
//                                    {"false_bool2", false},
//                                    {"true_bool", true},
//                                    {"true_bool2", true},
//                                    {"list", sb::cf::JsonArray{"string", "string1", "string2"}},
//                                    {"int_list", sb::cf::JsonArray{33, 22, 11}},
//                                    {"object", {{"inner", {{"object", "string"}}}}}};
//
//     EXPECT_EQ(provider->getConfiguration(), expected);
// }
//
// TEST_F(CommandLineConfigurationTest, ShouldLoadEmptyConfFromArgs)
// {
//     const char *argv[] = {
//         "exec/path",
//     };
//     int size = sizeof(argv) / sizeof(char *);
//     auto provider = sb::cf::CommandLineConfigurationSource::create(size, argv)->build(mock);
//
//     provider->load();
//
//     EXPECT_TRUE(provider->getConfiguration().empty());
// }
