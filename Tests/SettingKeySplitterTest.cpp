#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-param-util.h>
#include <random>
#include <string_view>
#include <tuple>

#include "SevenBit/Conf/Details/SettingKeySplitter.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "Utilities/ParamsTest.hpp"

class SettingKeySplitterTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    SettingKeySplitterTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SettingKeySplitterTest() {}

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

static Params<std::vector<std::string_view>, std::string> SettingsValues = {
    // String
    {{"option", "nested", "nested"}, "String"},
    {{"option"}, "StRinG"},
    {{"option", "nested"}, "str"},
    {{"option", "nested", "helloooooo", "nun"}, "str"},
    {{"option", "nested", "long", "nun", "long", "verryLong", "long", "long"}, "str"},
    {{"option", "3", "nested"}, "json"}};
PARAMS_TEST_COMBINED_4(SettingKeySplitterTest, ShouldSplitSettings, OptionPrefix, SettingsValues, TypeMarkers,
                       KeySeparators)
{
    const auto &[prefix, params, typeMark, separator] = GetParam();
    const auto &[key, type] = params;
    sb::cf::details::SettingKeySplitter splitter{OptionPrefix[1], KeySeparators[0], TypeMarkers[0], KeySeparators[1],
                                                 TypeMarkers[1]};

    sb::cf::details::SettingKeySplitter::Result expected{key, type};

    auto setting = prefix + sb::cf::details::utils::joinViews(key, separator) + typeMark + type;
    auto result = splitter.split(setting);
    EXPECT_EQ(result, expected);
}

PARAMS_TEST(SettingKeySplitterTest, ShouldSplitSettingsRandom, SettingsValues)
{
    const auto &[key, type] = GetParam();

    sb::cf::details::SettingKeySplitter splitter{OptionPrefix[1], KeySeparators[0], TypeMarkers[0], KeySeparators[1],
                                                 TypeMarkers[1]};

    sb::cf::details::SettingKeySplitter::Result expected{key, type};

    auto keys = randomJoin(key, KeySeparators);
    auto withOptionPrefix = randomJoin("", keys, OptionPrefix);
    auto setting = randomJoin(withOptionPrefix, type, TypeMarkers);

    auto result = splitter.split(setting);
    EXPECT_EQ(result, expected);
}

static Params<std::string_view, sb::cf::details::SettingKeySplitter::Result> ProblematicSettingsValues = {
    {"--option!!type", {{"option!"}, "type"}},
    {"--option!!", {{"option!"}, ""}},
    {"--option!!:inner!", {{"option!!", "inner"}, ""}},
    {"--option!!:inner:::!", {{"option!!", "inner", "", "", ""}, ""}},
    {":option:inner", {{"", "option", "inner"}, ""}},
    {":option!!:inner", {{"", "option!!", "inner"}, ""}},
    {":::!", {{"", "", "", ""}, ""}},
    {"::!:", {{"", "", "!", ""}, ""}},
    {":", {{"", ""}, ""}},
    {"__!", {{"", ""}, ""}},
    {"_______", {{"", "", ""}, ""}},
    {"__hello_____type", {{"", "hello", ""}, "type"}},
    {"__hello__type", {{"", "hello", "type"}, ""}},
};
PARAMS_TEST(SettingKeySplitterTest, ShouldSplitProblematicSettings, ProblematicSettingsValues)
{
    const auto &[setting, expected] = GetParam();

    sb::cf::details::SettingKeySplitter splitter{OptionPrefix[1], KeySeparators[0], TypeMarkers[0], KeySeparators[1],
                                                 TypeMarkers[1]};

    auto result = splitter.split(setting);
    EXPECT_EQ(result, expected);
}

static Params<std::string_view> FailSettingsValues = {"", "--"};
PARAMS_TEST(SettingKeySplitterTest, ShouldFailSplitSettings, FailSettingsValues)
{
    const auto &[setting] = GetParam();

    sb::cf::details::SettingKeySplitter splitter{OptionPrefix[1], KeySeparators[0], TypeMarkers[0], KeySeparators[1],
                                                 TypeMarkers[1]};

    EXPECT_ANY_THROW(splitter.split(setting));
}