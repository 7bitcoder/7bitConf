#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-param-util.h>
#include <iostream>
#include <string_view>
#include <tuple>

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/SettingParserBuilder.hpp"
#include "Utilities/ParamsTest.hpp"

class SettingParserTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    SettingParserTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SettingParserTest() {}

    static void TearDownTestSuite() {}
};
static OneParams<std::string> TypeMarkers = {"!", "___"};
static OneParams<std::string> OptionPrefix = {"", "--"};
static OneParams<std::string> KeySeparators = {":", "__"};

static Params<std::string, std::string, std::string, sb::cf::ISettingParser::Result> SimpleOptionsValues = {
    // String
    {"option", "String", "hello", {{"option"}, "hello"}},
    {"option", "StRinG", "hell\to", {{"option"}, "hell\to"}},
    {"option", "string", "hello", {{"option"}, "hello"}},
    // Bool
    {"option", "Bool", "true", {{"option"}, true}},
    {"option", "BoOl", "true", {{"option"}, true}},
    {"option", "BOOL", "true", {{"option"}, true}},
    {"option", "bool", "True", {{"option"}, true}},
    {"option", "bool", "TrUe", {{"option"}, true}},
    {"option", "bool", "1", {{"option"}, true}},
    {"option", "bool", " 1", {{"option"}, true}},
    {"option", "bool", "\t\n 1", {{"option"}, true}},
    {"option", "bool", "-1", {{"option"}, true}},
    {"option", "bool", "-1123", {{"option"}, true}},
    {"option", "bool", "1123", {{"option"}, true}},
    {"option", "bool", "false", {{"option"}, false}},
    {"option", "bool", "False", {{"option"}, false}},
    {"option", "bool", "FAlSe", {{"option"}, false}},
    {"option", "bool", "0", {{"option"}, false}},
    // Int
    {"option", "int", "1", {{"option"}, 1}},
    {"option", "int", " 1", {{"option"}, 1}},
    {"option", "int", "\t\n 1", {{"option"}, 1}},
    {"option", "int", "1", {{"option"}, 1}},
    {"option", "Int", "1", {{"option"}, 1}},
    {"option", "InT", "1", {{"option"}, 1}},
    {"option", "INT", "1", {{"option"}, 1}},
    {"option", "int", "-1234", {{"option"}, -1234}},
    {"option", "int", "1234", {{"option"}, 1234}},
    {"option", "int", "-223372036854775807", {{"option"}, -223372036854775807ll}},
    {"option", "int", "223372036854775807", {{"option"}, 223372036854775807ll}},
    // UInt
    {"option", "uint", "1", {{"option"}, 1}},
    {"option", "uint", " 1", {{"option"}, 1}},
    {"option", "uint", "\t\n 1", {{"option"}, 1}},
    {"option", "uInt", "1", {{"option"}, 1}},
    {"option", "uInT", "1", {{"option"}, 1}},
    {"option", "UINT", "1", {{"option"}, 1}},
    {"option", "uint", "1234", {{"option"}, 1234}},
    {"option", "uint", "223372036854775807", {{"option"}, 223372036854775807ll}},
    {"option", "uint", "17446744073709551615", {{"option"}, 17446744073709551615ull}},
    // Double
    {"option", "double", "1", {{"option"}, 1.0}},
    {"option", "double", " 1", {{"option"}, 1.0}},
    {"option", "double", "\t\n 1", {{"option"}, 1.0}},
    {"option", "Double", "1", {{"option"}, 1.0}},
    {"option", "dOuBle", "1", {{"option"}, 1.0}},
    {"option", "DOUBLE", "1", {{"option"}, 1.0}},
    {"option", "double", "1.123", {{"option"}, 1.123}},
    {"option", "double", "-12.21", {{"option"}, -12.21}},
    {"option", "double", "10000.11", {{"option"}, 10000.11}},
    {"option", "double", "-10000.11", {{"option"}, -10000.11}},
    // Null
    {"option", "null", "1", {{"option"}, sb::cf::json::null}},
    {"option", "null", "asdqwdwq", {{"option"}, sb::cf::json::null}},
    {"option", "Null", "1", {{"option"}, sb::cf::json::null}},
    {"option", "NULL", "1asdqwdwq", {{"option"}, sb::cf::json::null}},
    {"option", "NuLl", "1.123 asdqwdwq", {{"option"}, sb::cf::json::null}},
    // Json
    {"option", "json", "{\"hello\": 123456}", {{"option"}, {{"hello", 123456}}}},
    {"option", "Json", "{\"hello\": 123456}", {{"option"}, {{"hello", 123456}}}},
    {"option", "Json", "{\"hello\": 123456}", {{"option"}, {{"hello", 123456}}}},
    {"option", "JsOn", "{\"hello\": 123456}", {{"option"}, {{"hello", 123456}}}},
    {"option", "json", "{\"hello\": [1]}", {{"option"}, {{"hello", sb::cf::JsonArray{1}}}}},
    {"option", "json", "{\"hello\": null}", {{"option"}, {{"hello", sb::cf::json::null}}}},
};
PARAMS_TEST_COMBINED_3(SettingParserTest, ShouldParseSimpleOptions, OptionPrefix, SimpleOptionsValues, TypeMarkers)
{
    const auto &[prefix, params, typeMark] = GetParam();
    const auto &[name, type, value, expected] = params;
    auto parser = sb::cf::SettingParserBuilder{}.build();

    EXPECT_EQ((parser->parse(prefix + name + typeMark + type + "=" + value)), expected);
}

static Params<std::string, std::string, sb::cf::ISettingParser::Result> EmptyOptionsValues = {
    {"option", "string", {{"option"}, ""}},  {"option", "int", {{"option"}, 0}},
    {"option", "uint", {{"option"}, 0}},     {"option", "bool", {{"option"}, false}},
    {"option", "double", {{"option"}, 0.0}}, {"option", "null", {{"option"}, sb::cf::json::null}},
};
PARAMS_TEST_COMBINED_3(SettingParserTest, ShouldParseEmptyOptions, OptionPrefix, EmptyOptionsValues, TypeMarkers)
{
    auto &[prefix, params, typeMark] = GetParam();
    auto &[name, type, expected] = params;
    auto parser = sb::cf::SettingParserBuilder{}.build();
    EXPECT_EQ((parser->parse(prefix + name + typeMark + type)), expected);
}

TEST_F(SettingParserTest, ShouldParseEmptyJsonOption)
{
    auto parser = sb::cf::SettingParserBuilder{}.build();
    EXPECT_EQ((parser->parse("option!json")), (sb::cf::ISettingParser::Result{{"option"}, sb::cf::JsonValue{}}));
    EXPECT_EQ((parser->parse("--option!json")), (sb::cf::ISettingParser::Result{{"option"}, sb::cf::JsonValue{}}));
}

static OneParams<std::string> EmptyOptionValues = {"=hello", "=", ""};
PARAMS_TEST_COMBINED_2(SettingParserTest, ShouldFailParseEmptyOption, OptionPrefix, EmptyOptionValues)
{
    auto [prefix, option] = GetParam();

    auto parser = sb::cf::SettingParserBuilder{}.build();

    EXPECT_THROW(parser->parse(prefix + option), sb::cf::SettingParserException);
}

static Params<std::string, std::string, std::string> FailParseOptionValues = {
    // Int
    {"option", "int", "123 abcd"},
    {"option", "int", "123 "},
    {"option", "int", "123.123"},
    {"option", "int", " as 123 abcd"},
    {"option", "int", "abcd"},
    {"option", "int", " "},
    {"option", "int", ""},
    {"option", "int", "\n"},
    // UInt
    {"option", "uint", "123 abcd"},
    {"option", "uint", "123 "},
    {"option", "uint", "-12"},
    {"option", "uint", "123.123"},
    {"option", "uint", " as 123 abcd"},
    {"option", "uint", "abcd"},
    {"option", "uint", " "},
    {"option", "uint", ""},
    {"option", "uint", "\n"},
    // Bool
    {"option", "bool", "123 abcd"},
    {"option", "bool", "123.123"},
    {"option", "bool", " as 123 abcd"},
    {"option", "bool", "abcd"},
    {"option", "bool", " "},
    {"option", "bool", ""},
    {"option", "bool", "\n"},
    {"option", "bool", "ttruee"},
    {"option", "bool", "ffalsee"},
    {"option", "bool", "false "},
    {"option", "bool", "false "},
    {"option", "bool", " false"},
    // Double
    {"option", "double", "123 abcd"},
    {"option", "double", "123 "},
    {"option", "double", "123 "},
    {"option", "double", " as 123 abcd"},
    {"option", "double", "abcd"},
    {"option", "double", " "},
    {"option", "double", ""},
    {"option", "double", "\n"},
    // Json
    {"option", "json", "123 abcd"},
    {"option", "json", " as 123 abcd"},
    {"option", "json", "abcd"},
    {"option", "json", "{\"hello: 123}"},
    {"option", "json", "{\"hello\": sde123}"},
    {"option", "json", " "},
    {"option", "json", ""},
    {"option", "json", "\n"},
};
PARAMS_TEST_COMBINED_3(SettingParserTest, ShouldFailParseOption, OptionPrefix, FailParseOptionValues, TypeMarkers)
{
    auto &[prefix, params, typeMark] = GetParam();
    auto &[name, type, value] = params;
    auto parser = sb::cf::SettingParserBuilder{}.build();

    EXPECT_THROW(parser->parse(prefix + name + typeMark + type + "=" + value), sb::cf::SettingParserException);
}

static Params<std::vector<std::string>, std::string, std::string, sb::cf::JsonObject> DeepValues = {
    {{"option", "0", "deep"},
     "String",
     "hello",
     sb::cf::JsonObject{{"option", sb::cf::JsonArray{{{"deep", "hello"}}}}}},
    {{"option", "0", "0", "0"},
     "int",
     "123456",
     sb::cf::JsonObject{{"option", sb::cf::JsonArray(1, sb::cf::JsonArray(1, sb::cf::JsonArray{123456}))}}},
    {{"option", "0", "0", "0not"},
     "bool",
     "true",
     sb::cf::JsonObject{{"option", sb::cf::JsonArray(1, sb::cf::JsonArray(1, {{"0not", true}}))}}},
    {{"option", "inner", "inner", "inner", "inner", "inner"},
     "double",
     "1.123",
     sb::cf::JsonObject{
         {"option",
          sb::cf::JsonObject{
              {"inner",
               sb::cf::JsonObject{
                   {"inner",
                    sb::cf::JsonObject{
                        {"inner", sb::cf::JsonObject{{"inner", sb::cf::JsonObject{{"inner", 1.123}}}}}}}}}}}}},
};

// PARAMS_TEST_COMBINED_4(SettingParserTest, ShouldParseDeepOption, OptionPrefix, KeySeparators, DeepValues,
// TypeMarkers)
//{
//     auto parser = sb::cf::SettingParserBuilder{}.build();
//     auto &[prefix, separator, params, typeMark] = GetParam();
//     auto &[keys, type, value, expected] = params;
//
//     EXPECT_EQ(parser->parse(prefix + sb::cf::details::utils::join(keys, separator) + typeMark + type + "=" + value),
//               expected);
// }