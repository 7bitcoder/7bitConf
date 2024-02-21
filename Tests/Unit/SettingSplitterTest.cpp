#include <gtest/gtest.h>
#include <string_view>

#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/StringUtils.hpp"
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
    {"--", {{"--"}}},
    {"one", {{"one"}}},
    {"====", {{""}, std::nullopt, "==="}},
    {"option!!type=val", {{"option!"}, "type", "val"}},
    {"option!type=value", {{"option"}, "type", "value"}},
    {"option___type;value", {{"option"}, "type", "value"}},
    {"option=value", {{"option"}, std::nullopt, "value"}},
    {"option=", {{"option"}, std::nullopt, ""}},
    {"option!!type=", {{"option!"}, "type", ""}},
    {"option!!type", {{"option!"}, "type"}},
    {"option!!=type=val", {{"option!"}, "", "type=val"}},
    {"option!!:inner!=type=val", {{"option!!", "inner"}, "", "type=val"}},
    {"option!!:inner:::!=type=val", {{"option!!", "inner", "", "", ""}, "", "type=val"}},
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
    sb::cf::details::SettingSplitter splitter{{"=", ";"}, {"!", "___"}, {":", "__"}, true};

    EXPECT_EQ(splitter.split(setting), expected);
}

static OneParams<std::string> SettingSplittersData = {"=", ">", "===", "<<<<", "////"};

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
PARAMS_TEST_COMBINED_4(SettingSplitterTest, ShouldSplitWithDifferentSplitters, SettingSplittersData, TypeMarkersData,
                       KeySplittersData, SplitSettingSimpleData)
{
    const auto &[settingSplitter, typeMarker, keySplitter, setting] = GetParam();
    const auto &[keys, type, value] = setting;

    sb::cf::details::SettingSplitter splitter{{settingSplitter}, {typeMarker}, {keySplitter}, true};

    auto key = sb::cf::details::StringUtils::join(keys, keySplitter);
    auto fullSetting = key + typeMarker + type + settingSplitter + value;
    EXPECT_EQ(splitter.split(fullSetting), (sb::cf::ISettingSplitter::Result{keys, type, value}));
}

PARAMS_TEST_COMBINED_3(SettingSplitterTest, ShouldFailOnEmptyKeys, SettingSplittersData, TypeMarkersData,
                       KeySplittersData)
{
    const auto &[settingSplitter, typeMarker, keySplitter] = GetParam();

    sb::cf::details::SettingSplitter splitter{{settingSplitter}, {typeMarker}, {keySplitter}, false};

    auto key = sb::cf::details::StringUtils::join({"", "", ""}, keySplitter);
    auto fullSetting = key + typeMarker + "type" + settingSplitter + "value";
    auto act = [&] { auto _ = splitter.split(fullSetting); };
    EXPECT_THROW(act(), sb::cf::ConfigException);
}

static Params<std::vector<std::string_view>, std::vector<std::string_view>, std::vector<std::string_view>,
              sb::cf::ISettingSplitter::Result>
    EmptySplittersData = {
        {{"=", ";"}, {"!", "___"}, {":", "__"}, {{"key", "deep"}, "int", "value"}},
        {{}, {"!", "___"}, {":", "__"}, {{"key", "deep"}, "int=value"}},
        {{"=", ";"}, {}, {":", "__"}, {{"key", "deep!int"}, std::nullopt, "value"}},
        {{"=", ";"}, {"!", "___"}, {}, {{"key:deep"}, "int", "value"}},
        {{}, {"!", "___"}, {":", "__"}, {{"key", "deep"}, "int=value"}},
        {{"=", ";"}, {}, {":", "__"}, {{"key", "deep!int"}, std::nullopt, "value"}},
        {{"=", ";"}, {"!", "___"}, {}, {{"key:deep"}, "int", "value"}},
        {{}, {}, {":", "__"}, {{"key", "deep!int=value"}}},
        {{}, {"!", "___"}, {}, {{"key:deep"}, "int=value"}},
        {{"=", ";"}, {}, {}, {{"key:deep!int"}, std::nullopt, "value"}},
        {{}, {}, {":", "__"}, {{"key", "deep!int=value"}}},
        {{}, {"!", "___"}, {}, {{"key:deep"}, "int=value"}},
        {{"=", ";"}, {}, {}, {{"key:deep!int"}, std::nullopt, "value"}},
        {{}, {}, {}, {{"key:deep!int=value"}}},
        {{}, {}, {}, {{"key:deep!int=value"}}},

};
PARAMS_TEST(SettingSplitterTest, ShouldSplitWithEmptySplitters, EmptySplittersData)
{
    const auto &[settingSplitters, typeMarkers, keySplitters, expected] = GetParam();

    sb::cf::details::SettingSplitter splitter{settingSplitters, typeMarkers, keySplitters, true};

    EXPECT_EQ(splitter.split("key:deep!int=value"), expected);
}
