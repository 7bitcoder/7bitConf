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
    sb::cf::details::SettingSplitter splitter{{"--"}, {"="}, {"!", "___"}, {":", "__"}};

    EXPECT_EQ(splitter.split(setting), expected);
}
