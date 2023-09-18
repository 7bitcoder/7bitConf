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

    ~SettingSplitterTest() {}

    static void TearDownTestSuite() {}
};

static Params<std::string_view, sb::cf::details::SettingSplitter::Result> SettingValues = {
    {"--", {"--"}},
    {"one", {"one"}},
    {"--====", {"--", "==="}},
    {"--option!!type=val", {"--option!!type", "val"}},
    {"--option!!type=", {"--option!!type", ""}},
    {"--option!!type", {"--option!!type"}},
    {"--option!!=type=val", {"--option!!", "type=val"}},
    {"--option!!:inner!=type=val", {"--option!!:inner!", "type=val"}},
    {"--option!!:inner:::!=type=val", {"--option!!:inner:::!", "type=val"}},
    {":option:inner=type=val", {":option:inner", "type=val"}},
    {":option:inner=value", {":option:inner", "value"}},
    {":option:inner=value::!", {":option:inner", "value::!"}},
    {":option!!:inner=value::!", {":option!!:inner", "value::!"}},
    {":::!=value::!", {":::!", "value::!"}},
    {"::!:=value::!", {"::!:", "value::!"}},
    {":=:!:=value::!", {":", ":!:=value::!"}},
    {"__=:!___=value::!", {"__", ":!___=value::!"}},
    {"_______=value", {"_______", "value"}},
    {"__hello_____type=value", {"__hello_____type", "value"}},
    {"__hello__type=value", {"__hello__type", "value"}},
};
PARAMS_TEST(SettingSplitterTest, ShouldSplitSetting, SettingValues)
{
    const auto &[setting, expected] = GetParam();
    sb::cf::details::SettingSplitter splitter{"="};

    EXPECT_EQ(splitter.split(setting), expected);
}

static Params<std::string, std::string, sb::cf::details::SettingSplitter::Result> SimpleSettingValues = {
    {"key", "value", {"key", "value"}},
    {"key", "", {"key", ""}},
    {"--option", "value", {"--option", "value"}},
    {"--option!type", "", {"--option!type", ""}},
};
static Params<std::string> AlternativeSplittersValues = {
    "=", ">>>", ";;;", "_________", "12", "======================", "||", "\\", "/,", "??", "........", "~``~~`"};
PARAMS_TEST_COMBINED_2(SettingSplitterTest, ShouldSplitSettingWithAlternativeSpliters, SimpleSettingValues,
                       AlternativeSplittersValues)
{
    const auto &[setting, divider] = GetParam();
    const auto &[key, value, expected] = setting;
    const auto &[dividerStr] = divider;

    sb::cf::details::SettingSplitter splitter{dividerStr};

    EXPECT_EQ(splitter.split(key + dividerStr + value), expected);
}
