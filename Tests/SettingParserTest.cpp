#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-param-util.h>
#include <iostream>
#include <string_view>
#include <tao/json/type.hpp>
#include <tuple>

#include "SevenBit/Config/Details/SettingParser.hpp"
#include "SevenBit/Config/Details/Utils.hpp"
#include "SevenBit/Config/Exceptions.hpp"
#include "SevenBit/Config/Json.hpp"
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

static Params<std::string, std::string, std::string, sb::cf::JsonObject> SimpleOptionsValues = {
    // String
    {"option", "String", "hello", sb::cf::JsonObject{{"option", "hello"}}},
    {"option", "StRinG", "hell\to", sb::cf::JsonObject{{"option", "hell\to"}}},
    {"option", "string", "hello", sb::cf::JsonObject{{"option", "hello"}}},
    // Bool
    {"option", "Bool", "true", sb::cf::JsonObject{{"option", true}}},
    {"option", "BoOl", "true", sb::cf::JsonObject{{"option", true}}},
    {"option", "BOOL", "true", sb::cf::JsonObject{{"option", true}}},
    {"option", "bool", "True", sb::cf::JsonObject{{"option", true}}},
    {"option", "bool", "TrUe", sb::cf::JsonObject{{"option", true}}},
    {"option", "bool", "1", sb::cf::JsonObject{{"option", true}}},
    {"option", "bool", " 1", sb::cf::JsonObject{{"option", true}}},
    {"option", "bool", "\t\n 1", sb::cf::JsonObject{{"option", true}}},
    {"option", "bool", "-1", sb::cf::JsonObject{{"option", true}}},
    {"option", "bool", "-1123", sb::cf::JsonObject{{"option", true}}},
    {"option", "bool", "1123", sb::cf::JsonObject{{"option", true}}},
    {"option", "bool", "false", sb::cf::JsonObject{{"option", false}}},
    {"option", "bool", "False", sb::cf::JsonObject{{"option", false}}},
    {"option", "bool", "FAlSe", sb::cf::JsonObject{{"option", false}}},
    {"option", "bool", "0", sb::cf::JsonObject{{"option", false}}},
    // Int
    {"option", "int", "1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "int", " 1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "int", "\t\n 1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "int", "1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "Int", "1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "InT", "1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "INT", "1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "int", "-1234", sb::cf::JsonObject{{"option", -1234}}},
    {"option", "int", "1234", sb::cf::JsonObject{{"option", 1234}}},
    {"option", "int", "-223372036854775807", sb::cf::JsonObject{{"option", -223372036854775807ll}}},
    {"option", "int", "223372036854775807", sb::cf::JsonObject{{"option", 223372036854775807ll}}},
    // UInt
    {"option", "uint", "1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "uint", " 1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "uint", "\t\n 1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "uInt", "1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "uInT", "1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "UINT", "1", sb::cf::JsonObject{{"option", 1}}},
    {"option", "uint", "1234", sb::cf::JsonObject{{"option", 1234}}},
    {"option", "uint", "223372036854775807", sb::cf::JsonObject{{"option", 223372036854775807ll}}},
    {"option", "uint", "17446744073709551615", sb::cf::JsonObject{{"option", 17446744073709551615ull}}},
    // Double
    {"option", "double", "1", sb::cf::JsonObject{{"option", 1.0}}},
    {"option", "double", " 1", sb::cf::JsonObject{{"option", 1.0}}},
    {"option", "double", "\t\n 1", sb::cf::JsonObject{{"option", 1.0}}},
    {"option", "Double", "1", sb::cf::JsonObject{{"option", 1.0}}},
    {"option", "dOuBle", "1", sb::cf::JsonObject{{"option", 1.0}}},
    {"option", "DOUBLE", "1", sb::cf::JsonObject{{"option", 1.0}}},
    {"option", "double", "1.123", sb::cf::JsonObject{{"option", 1.123}}},
    {"option", "double", "-12.21", sb::cf::JsonObject{{"option", -12.21}}},
    {"option", "double", "10000.11", sb::cf::JsonObject{{"option", 10000.11}}},
    {"option", "double", "-10000.11", sb::cf::JsonObject{{"option", -10000.11}}},
    // Null
    {"option", "null", "1", sb::cf::JsonObject{{"option", sb::cf::json::null}}},
    {"option", "null", "asdqwdwq", sb::cf::JsonObject{{"option", sb::cf::json::null}}},
    {"option", "Null", "1", sb::cf::JsonObject{{"option", sb::cf::json::null}}},
    {"option", "NULL", "1asdqwdwq", sb::cf::JsonObject{{"option", sb::cf::json::null}}},
    {"option", "NuLl", "1.123 asdqwdwq", sb::cf::JsonObject{{"option", sb::cf::json::null}}},
    // Json
    {"option", "json", "{\"hello\": 123456}", sb::cf::JsonObject{{"option", {{"hello", 123456}}}}},
    {"option", "Json", "{\"hello\": 123456}", sb::cf::JsonObject{{"option", {{"hello", 123456}}}}},
    {"option", "Json", "{\"hello\": 123456}", sb::cf::JsonObject{{"option", {{"hello", 123456}}}}},
    {"option", "JsOn", "{\"hello\": 123456}", sb::cf::JsonObject{{"option", {{"hello", 123456}}}}},
    {"option", "json", "{\"hello\": [1]}", sb::cf::JsonObject{{"option", {{"hello", sb::cf::JsonArray{1}}}}}},
    {"option", "json", "{\"hello\": null}", sb::cf::JsonObject{{"option", {{"hello", sb::cf::json::null}}}}},
};
PARAMS_TEST_COMBINED_3(SettingParserTest, ShouldParseSimpleOptions, OptionPrefix, SimpleOptionsValues, TypeMarkers)
{
    const auto &[prefix, params, typeMark] = GetParam();
    const auto &[name, type, value, expected] = params;
    sb::cf::details::SettingParser parser;

    EXPECT_EQ((parser.parseSetting(prefix + name + typeMark + type + "=" + value)), expected);
}

static Params<std::string, std::string, sb::cf::JsonObject> EmptyOptionsValues = {
    {"option", "string", sb::cf::JsonObject{{"option", ""}}},
    {"option", "int", sb::cf::JsonObject{{"option", 0}}},
    {"option", "uint", sb::cf::JsonObject{{"option", 0}}},
    {"option", "bool", sb::cf::JsonObject{{"option", false}}},
    {"option", "double", sb::cf::JsonObject{{"option", 0.0}}},
    {"option", "null", sb::cf::JsonObject{{"option", sb::cf::json::null}}},
};
PARAMS_TEST_COMBINED_3(SettingParserTest, ShouldParseEmptyOptions, OptionPrefix, EmptyOptionsValues, TypeMarkers)
{
    auto &[prefix, params, typeMark] = GetParam();
    auto &[name, type, expected] = params;
    sb::cf::details::SettingParser parser;
    EXPECT_EQ((parser.parseSetting(prefix + name + typeMark + type)), expected);
}

TEST_F(SettingParserTest, ShouldParseEmptyJsonOption)
{
    sb::cf::details::SettingParser parser;
    EXPECT_EQ((parser.parseSetting("option!json")), (sb::cf::JsonObject{{"option", sb::cf::JsonValue{}}}));
    EXPECT_EQ((parser.parseSetting("--option!json")), (sb::cf::JsonObject{{"option", sb::cf::JsonValue{}}}));
}

static OneParams<std::string> EmptyOptionValues = {"=hello", "=", ""};
PARAMS_TEST_COMBINED_2(SettingParserTest, ShouldFailParseEmptyOption, OptionPrefix, EmptyOptionValues)
{
    auto [prefix, option] = GetParam();

    sb::cf::details::SettingParser parser;

    EXPECT_THROW(parser.parseSetting(prefix + option), sb::cf::SettingParserException);
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
    {"option", "uint", "-123"},
    {"option", "uint", "123 "},
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
    sb::cf::details::SettingParser parser;

    EXPECT_THROW(parser.parseSetting(prefix + name + typeMark + type + "=" + value), sb::cf::SettingParserException);
}

static Params<size_t, std::string, std::string, sb::cf::JsonValue> ListValues = {
    // Int
    {0, "int", "123", sb::cf::JsonValue(123)},
    {1, "int", "123", sb::cf::JsonValue(123)},
    {2, "int", "123", sb::cf::JsonValue(123)},
    {10, "int", "123", sb::cf::JsonValue(123)},
    {0, "uint", "123", sb::cf::JsonValue(123)},
    {1, "uint", "123", sb::cf::JsonValue(123)},
    {2, "uint", "123", sb::cf::JsonValue(123)},
    {10, "uint", "123", sb::cf::JsonValue(123)},
    {0, "bool", "true", sb::cf::JsonValue(true)},
    {1, "bool", "true", sb::cf::JsonValue(true)},
    {2, "bool", "true", sb::cf::JsonValue(true)},
    {10, "bool", "true", sb::cf::JsonValue(true)},
    {0, "string", "testStr", sb::cf::JsonValue("testStr")},
    {1, "string", "testStr", sb::cf::JsonValue("testStr")},
    {2, "string", "testStr", sb::cf::JsonValue("testStr")},
    {10, "string", "testStr", sb::cf::JsonValue("testStr")},
    {0, "null", "", sb::cf::JsonValue(sb::cf::json::null)},
    {1, "null", "", sb::cf::JsonValue(sb::cf::json::null)},
    {2, "null", "", sb::cf::JsonValue(sb::cf::json::null)},
    {10, "null", "", sb::cf::JsonValue(sb::cf::json::null)},
    {0, "double", "1.12", sb::cf::JsonValue(1.12)},
    {1, "double", "1.12", sb::cf::JsonValue(1.12)},
    {2, "double", "1.12", sb::cf::JsonValue(1.12)},
    {10, "double", "1.12", sb::cf::JsonValue(1.12)},
    {0, "json", "1.12", sb::cf::JsonValue(1.12)},
    {1, "json", "1.12", sb::cf::JsonValue(1.12)},
    {2, "json", "1.12", sb::cf::JsonValue(1.12)},
    {10, "json", "1.12", sb::cf::JsonValue(1.12)},
};
PARAMS_TEST_COMBINED_4(SettingParserTest, ShouldParseListValueOption, OptionPrefix, KeySeparators, ListValues,
                       TypeMarkers)
{
    sb::cf::details::SettingParser parser;
    auto &[prefix, separator, params, typeMark] = GetParam();
    auto &[index, type, value, expectedValue] = params;

    sb::cf::JsonArray expectedArray(index + 1, sb::cf::JsonValue());
    expectedArray[index] = expectedValue;

    EXPECT_EQ(parser.parseSetting(prefix + "array" + separator + std::to_string(index) + typeMark + type + "=" + value),
              (sb::cf::JsonObject{{"array", std::move(expectedArray)}}));
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
     sb::cf::JsonObject{{"option", {{"inner", {{"inner", {{"inner", {{"inner", {{"inner", 1.123}}}}}}}}}}}}},
};

PARAMS_TEST_COMBINED_4(SettingParserTest, ShouldParseDeepOption, OptionPrefix, KeySeparators, DeepValues, TypeMarkers)
{
    sb::cf::details::SettingParser parser;
    auto &[prefix, separator, params, typeMark] = GetParam();
    auto &[keys, type, value, expected] = params;

    EXPECT_EQ(parser.parseSetting(prefix + sb::cf::utils::join(keys, separator) + typeMark + type + "=" + value),
              expected);
}