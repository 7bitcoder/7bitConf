#include "SevenBit/Conf/Details/Utils.hpp"
#include "Utilities/ParamsTest.hpp"
#include <cstddef>
#include <gtest/gtest.h>
#include <iostream>
#include <string_view>
#include <utility>

class UtilsTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    UtilsTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~UtilsTest() {}

    static void TearDownTestSuite() {}
};

Params<std::string, std::string, std::vector<std::string_view>, int> SplitStrData{
    {"", "/", {""}, -1},
    {"first:sec:for:5:6", "/", {"first:sec:for:5:6"}, -1},
    {"first:sec:for:5:6", ":", {"first", "sec", "for", "5", "6"}, -1},
    {"first123++/?sec123++/?for123++/?5123++/?6", "123++/?", {"first", "sec", "for", "5", "6"}, -1},
    {":first:sec:for:5:6", ":", {"", "first", "sec", "for", "5", "6"}, -1},
    {"first::sec:for:5:6", ":", {"first", "", "sec", "for", "5", "6"}, -1},
    {"first:sec:for:5:6:", ":", {"first", "sec", "for", "5", "6", ""}, -1},
    {"first:sec:for:5:6:", ":", {"first", "sec:for:5:6:"}, 2},
    {"first:sec:for:5:6:", ":", {"first", "sec", "for:5:6:"}, 3},
    {"first:sec:for:5:6", ":", {"first", "sec", "for", "5", "6"}, 30},
};
PARAMS_TEST(UtilsTest, ShouldSplitString, SplitStrData)
{
    auto &[string, delim, expected, max] = GetParam();
    if (max < 0)
    {
        EXPECT_EQ(sb::cf::details::utils::split(string, delim), expected);
    }
    EXPECT_EQ(sb::cf::details::utils::split(string, delim, max), expected);
}

Params<std::vector<std::string_view>, std::string, std::string> JoinStrData{
    {{""}, "/", ""},
    {{"first:sec:for:5:6"}, "/", "first:sec:for:5:6"},
    {{"first", "sec", "for", "5", "6"}, ":", "first:sec:for:5:6"},
    {{"first", "sec", "for", "5", "6"}, "123++/?", "first123++/?sec123++/?for123++/?5123++/?6"},
    {{"", "first", "sec", "for", "5", "6"}, ":", ":first:sec:for:5:6"},
    {{"first", "", "sec", "for", "5", "6"}, ":", "first::sec:for:5:6"},
    {{"first", "sec", "for", "5", "6", ""}, ":", "first:sec:for:5:6:"},
    {{"first", "sec:for:5:6:"}, ":", "first:sec:for:5:6:"},
    {{"first", "sec", "for:5:6:"}, ":", "first:sec:for:5:6:"},
    {{"first", "sec", "for", "5", "6"}, ":", "first:sec:for:5:6"},
};
PARAMS_TEST(UtilsTest, ShouldJoinStrings, JoinStrData)
{
    auto &[strings, delim, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::joinViews(strings, delim), expected);
}

Params<std::string_view, std::string_view, bool> StartsWithData{
    {"1234567", "123", true}, {"1234567", "", true}, {"1234567", "1", true}, {"123", "123", true},
    {"123", "1234", false},   {"123", "890", false}, {"123", "1245", false}, {"1234567", "234", false},
    {"", "234", false},       {"123", "234", false},
};
PARAMS_TEST(UtilsTest, ShouldStartsWith, StartsWithData)
{
    auto &[string, search, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::startsWith(string, search), expected);
}

Params<std::string_view, std::string_view, bool> IgnoreCaseStartsWithData{
    {"AbcdeFsd", "abC", true}, {"AbcdeFsd", "", true},   {"1234567", "1", true}, {"abcd", "A", true},
    {"abcd", "ab", true},      {"abcd", "abcdE", false}, {"123", "890", false},  {"123", "1245", false},
    {"1234567", "234", false}, {"", "234", false},       {"123", "234", false},
};
PARAMS_TEST(UtilsTest, ShouldIgnoreCaseStartsWith, IgnoreCaseStartsWithData)
{
    auto &[string, search, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::ignoreCaseStartsWith(string, search), expected);
}

Params<std::string_view, std::string_view, bool> EndsWithData{
    {"1234567", "567", true}, {"1234567", "", true}, {"1234567", "7", true}, {"123", "123", true},
    {"123", "1234", false},   {"123", "890", false}, {"123", "1245", false}, {"1234567", "234", false},
    {"", "234", false},       {"123", "234", false},
};
PARAMS_TEST(UtilsTest, ShouldEndsWith, EndsWithData)
{
    auto &[string, search, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::endsWith(string, search), expected);
}

Params<std::string_view, std::string_view, bool> IgnoreCaseEndsWithData{
    {"AbcdeFsd", "fSD", true}, {"AbcdeFsd", "", true}, {"1234567", "7", true}, {"abcd", "D", true},
    {"abcd", "SD", false},     {"123", "890", false},  {"123", "1245", false}, {"1234567", "234", false},
    {"", "234", false},        {"123", "234", false},
};
PARAMS_TEST(UtilsTest, ShouldIgnoreCaseEndsWith, IgnoreCaseEndsWithData)
{
    auto &[string, search, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::ignoreCaseEndsWith(string, search), expected);
}

TEST_F(UtilsTest, ShouldReplaceAllStrings)
{
    std::string str = "bup__hup__mub";
    EXPECT_TRUE(sb::cf::details::utils::replaceAll(str, "__", ":"));
    EXPECT_EQ(str, "bup:hup:mub");
}

Params<std::string_view, std::string_view, bool> IgnoreCaseEqualsData{
    {"", "", true},
    {"I", "i", true},
    {"i", "I", true},
    {"123@#", "123@#", true},
    {"abcdef", "ABCDEF", true},
    {"abcDEF", "abcdef", true},
    {"abcDEF", "abcded", false},
    {"", "abcded", false},
    {"BA", "ab", false},
    {"ab", "ab\n", false},
    {"1222", "12", false},
};
PARAMS_TEST(UtilsTest, ShouldIgnoreCaseCompareStrings, IgnoreCaseEqualsData)
{
    auto &[string, search, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::ignoreCaseEquals(string, search), expected);
}

Params<std::string_view, bool> CheckNumberStringsData{
    {"123", true},     {"1", true},         {"0912837545234123", true}, {"asd", false},
    {"", false},       {"alk1", false},     {"1223-", false},           {"1223#", false},
    {"1223+=", false}, {"1223.123", false}, {"1223.123", false},
};
PARAMS_TEST(UtilsTest, ShouldCheckNumberStrings, CheckNumberStringsData)
{
    auto &[string, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::isNumberString(string), expected);
}

Params<std::string_view, bool, std::pair<bool, int>> ConvertToNumberIntData{
    {"123", true, {true, 123}},     {"-123", true, {true, -123}},   {"00123", true, {true, 123}},
    {"123 23", false, {true, 123}}, {"123.23", false, {true, 123}}, {"123adawadwa", false, {true, 123}},
};
PARAMS_TEST(UtilsTest, ShouldConvertToIntNumber, ConvertToNumberIntData)
{
    auto &[string, full, expected] = GetParam();
    auto [success, result] = sb::cf::details::utils::tryStringTo<int>(string, full);

    EXPECT_EQ(success, expected.first);
    if (success)
    {
        EXPECT_EQ(result, expected.second);
    }
}

Params<std::string_view, bool, std::pair<bool, double>> ConvertToNumberDoubleData{
    {"123.123", true, {true, 123.123}},      {"  123.123", true, {true, 123.123}},   {"-123.22", true, {true, -123.22}},
    {"00123.2", true, {true, 123.2}},        {" 123.23asdw", false, {true, 123.23}}, {"123.23", false, {true, 123.23}},
    {"123.1adawadwa", false, {true, 123.1}},
};
PARAMS_TEST(UtilsTest, ShouldConvertToDoubleNumber, ConvertToNumberDoubleData)
{
    auto &[string, full, expected] = GetParam();
    auto [success, result] = sb::cf::details::utils::tryStringTo<double>(string, full);

    EXPECT_EQ(success, expected.first);
    if (success)
    {
        EXPECT_EQ(result, expected.second);
    }
}

Params<std::string_view, bool, std::pair<bool, bool>> ConvertToBoolData{
    {"true", true, {true, true}},      {"false", true, {true, false}},    {"-123.22", true, {false, false}},
    {"00123.2", true, {false, false}}, {"0", true, {true, false}},        {"0", true, {true, false}},
    {"0 asdad", false, {true, false}}, {"12 asdad", false, {true, true}}, {" 12", true, {true, true}},
    {"ttrue", true, {false, true}},
};
PARAMS_TEST(UtilsTest, ShouldConvertToBool, ConvertToBoolData)
{
    auto &[string, full, expected] = GetParam();
    auto [success, result] = sb::cf::details::utils::tryStringTo<bool>(string, full);

    EXPECT_EQ(success, expected.first);
    if (success)
    {
        EXPECT_EQ(result, expected.second);
    }
}