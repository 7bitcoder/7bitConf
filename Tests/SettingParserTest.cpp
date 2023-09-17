#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-param-util.h>
#include <random>
#include <string_view>
#include <tuple>

#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
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

std::string randomJoin(const std::string &a, const std::string &b, const std::vector<std::string> &delims)
{
    static std::random_device _device; // non-deterministic generator
    static std::mt19937 _generator(_device());

    auto index = _generator() % (delims.size() - 1);
    const auto &delim = delims[index];
    return a + delim + b;
}

std::string randomJoin(const std::vector<std::string_view> &toJoin, const std::vector<std::string> &delims)
{
    std::string result = std::string{toJoin[0]};
    for (size_t i = 1; i < toJoin.size(); i++)
    {
        result = randomJoin(result, std::string{toJoin[i]}, delims);
    }
    return result;
}

static OneParams<std::string> TypeMarkers = {"!", "___"};
static OneParams<std::string> OptionPrefix = {"", "--"};
static OneParams<std::string> KeySeparators = {":", "__"};

static Params<std::vector<std::string_view>, std::string, std::string> OptionsValues = {
    // String
    {{"option", "nested", "nested"}, "String", "hello"},
    {{"option"}, "StRinG", "hell\to"},
    {{"option", "nested"}, "str", "hello"},
    {{"option", "nested", "helloooooo", "nun"}, "str", ""},
    {{"option", "nested", "long", "nun", "long", "verryLong", "long", "long"}, "str", "value"},
    {{"option", "3", "nested"}, "json", "{\"hello\": null}"}};
PARAMS_TEST_COMBINED_4(SettingParserTest, ShouldParseOptions, OptionPrefix, OptionsValues, TypeMarkers, KeySeparators)
{
    const auto &[prefix, params, typeMark, separator] = GetParam();
    const auto &[key, type, value] = params;
    sb::cf::details::SettingParser parser;

    sb::cf::details::SettingParserResult expected{key, type, value};

    auto setting = prefix + sb::cf::details::utils::joinViews(key, separator) + typeMark + type + "=" + value;
    auto result = parser.parse(setting);
    EXPECT_EQ(result, expected);
}

PARAMS_TEST(SettingParserTest, ShouldParseOptionsRandom, OptionsValues)
{
    const auto &[key, type, value] = GetParam();

    sb::cf::details::SettingParser parser;

    sb::cf::details::SettingParserResult expected{key, type, value};

    auto keys = randomJoin(key, KeySeparators);
    auto withOptionPrefix = randomJoin("", keys, OptionPrefix);
    auto setting = randomJoin(withOptionPrefix, type, TypeMarkers) + "=" + value;

    auto result = parser.parse(setting);
    EXPECT_EQ(result, expected);
}

static Params<std::string_view, sb::cf::details::SettingParserResult> ProblematicOptionsValues = {
    {"--option!!type=val", {"option!", "type", "val"}},
    {"--option!!=type=val", {"option!", "", "type=val"}},
    {"--option!!:inner!=type=val", {{"option!!", "inner"}, "", "type=val"}},
    {"--option!!:inner:::!=type=val", {{"option!!", "inner", "", "", ""}, "", "type=val"}},
    {":option:inner=type=val", {{"", "option", "inner"}, "", "type=val"}},
    {":option:inner=value", {{"", "option", "inner"}, "", "value"}},
    {":option:inner=value::!", {{"", "option", "inner"}, "", "value::!"}},
    {":option!!:inner=value::!", {{"", "option!!", "inner"}, "", "value::!"}},
    {":::!=value::!", {{"", "", "", ""}, "", "value::!"}},
    {"::!:=value::!", {{"", "", "!", ""}, "", "value::!"}},
    {":=:!:=value::!", {{"", ""}, "", ":!:=value::!"}},
    {"__=:!___=value::!", {{"", ""}, "", ":!___=value::!"}},
    {"_______=value", {{"", "", ""}, "", "value"}},
    {"__hello_____type=value", {{"", "hello", ""}, "type", "value"}},
    {"__hello__type=value", {{"", "hello", "type"}, "", "value"}},
};
PARAMS_TEST(SettingParserTest, ShouldParseProblematicOptions, ProblematicOptionsValues)
{
    const auto &[setting, expected] = GetParam();
    sb::cf::details::SettingParser parser;

    auto result = parser.parse(setting);
    EXPECT_EQ(result, expected);
}

static Params<std::string_view> FailOptionsValues = {"", "--",
                                                     "="
                                                     "--="};
PARAMS_TEST(SettingParserTest, ShouldFailParseOptions, FailOptionsValues)
{
    const auto &[setting] = GetParam();

    sb::cf::details::SettingParser parser;

    EXPECT_ANY_THROW(parser.parse(setting));
}