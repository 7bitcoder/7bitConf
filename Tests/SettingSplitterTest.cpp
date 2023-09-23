#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-param-util.h>
#include <random>
#include <string_view>
#include <tuple>

#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "Utilities/ParamsTest.hpp"

class SettingSplitterTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    SettingSplitterTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

static Params<std::string_view, sb::cf::details::SettingSplitter::Result> SplitSettingData = {
    {"--", {{""}}},
    {"one", {{"one"}}},
    {"--====", {{""}, std::nullopt, "==="}},
    {"--option!!type=val", {{"option!"}, "type", "val"}},
    {"--option!type=value", {{"option"}, "type", "value"}},
    {"//option___type;value", {{"option"}, "type", "value"}},
    {"--option=value", {{"option"}, std::nullopt, "value"}},
    {"--option=", {{"option"}, std::nullopt, ""}},
    {"--option!!type=", {{"option!"}, "type", ""}},
    {"--option!!type", {{"option!"}, "type"}},
    {"--option!!=type=val", {{"option!"}, "", "type=val"}},
    {"--option!!:inner!=type=val", {{"option!!", "inner"}, "", "type=val"}},
    {"--option!!:inner:::!=type=val", {{"option!!", "inner", "", "", ""}, "", "type=val"}},
    {":option:inner=type=val", {{"", "option", "inner"}, std::nullopt, "type=val"}},
    {"!!option:inner=type=val", {{"!"}, "option:inner", "type=val"}},
    {":option:inner=value", {{"", "option", "inner"}, std::nullopt, "value"}},
    {":option:inner=value::!", {{"", "option", "inner"}, std::nullopt, "value::!"}},
    {":option!!:inner=value::!", {{"", "option!"}, ":inner", "value::!"}},
    {":::!=value::!", {{"", "", "", ""}, "", "value::!"}},
    {"::!:=value::!", {{"", "", ""}, ":", "value::!"}},
    {":=:!:=value::!", {{"", ""}, std::nullopt, ":!:=value::!"}},
    {"__=:!___=value::!", {{"", ""}, std::nullopt, ":!___=value::!"}},
    {"_______=value", {{"", "", ""}, "", "value"}},
    {"__hello_____type=value", {{"", "hello", ""}, "type", "value"}},
    {"__hello__type=value", {{"", "hello", "type"}, std::nullopt, "value"}},
};
PARAMS_TEST(SettingSplitterTest, ShouldSplitSetting, SplitSettingData)
{
    const auto &[setting, expected] = GetParam();
    sb::cf::details::SettingSplitter splitter{{"--", "//"}, {"=", ";"}, {"!", "___"}, {":", "__"}};

    EXPECT_EQ(splitter.split(setting), expected);
}

static OneParams<std::string> SettingSplittersData = {"=", ">", "===", "<<<<", "////"};

static OneParams<std::string> SettingPrefixesData = {":", ";", "%%$", "***", "++"};

static OneParams<std::string> KeySplittersData = {":", ";", "__", "{}", "\\"};

static OneParams<std::string> TypeMarkersData = {"!", "@", ":", "[]", "{}"};

static Params<std::vector<std::string_view>, std::string, std::string> SplitSettingSimpleData = {
    {{"key1", "key2", "key3"}, "string", "value"},
    {{"key1"}, "string", "value"},
    {{""}, "", ""},
    {{""}, "type", ""},
    {{"key"}, "", ""},
    {{""}, "", "value"},
};
PARAMS_TEST_COMBINED_5(SettingSplitterTest, ShouldSplitWithDifferentSplitters, SettingPrefixesData,
                       SettingSplittersData, TypeMarkersData, KeySplittersData, SplitSettingSimpleData)
{
    const auto &[prefix, settingSplitter, typeMarker, keySplitter, setting] = GetParam();
    const auto &[keys, type, value] = setting;

    sb::cf::details::SettingSplitter splitter{{prefix}, {settingSplitter}, {typeMarker}, {keySplitter}};
    std::string key;
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        if (it != keys.begin())
        {
            key += keySplitter;
        }
        key += *it;
    }

    auto fullSetting = prefix + key + typeMarker + type + settingSplitter + value;
    EXPECT_EQ(splitter.split(fullSetting), (sb::cf::ISettingSplitter::Result{keys, type, value}));
}