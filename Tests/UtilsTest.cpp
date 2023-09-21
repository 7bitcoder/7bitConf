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

    static void TearDownTestSuite() {}
};

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

Params<std::string_view, std::string_view, bool> IgnoreCaseEqualsData{
    {"", "", true},
    {"I", "i", true},
    {"i", "I", true},
    {"123@#", "123@#", true},
    {"abcdef", "ABCDEF", true},
    {"abcDEF", "abcdef", true},
    {"abcDEF", "ABCdef", true},
    {"abcDEF12##", "abCdef12##", true},
    {"", "abcded", false},
    {"BA", "ab", false},
    {"ab", "ab\n", false},
    {"1222", "12", false},
};
PARAMS_TEST(UtilsTest, ShouldIgnoreCaseCompareStrings, IgnoreCaseEqualsData)
{
    auto &[string, search, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::ignoreCaseEqual(string, search), expected);
}

Params<std::string_view, size_t, std::string_view, bool> ContainsAtData{
    {"", 0, "", false},          {"hello", 0, "", false},    {"", 12, "", false},
    {"i", 0, "i", true},         {"index", 0, "in", true},   {"index", 3, "in", false},
    {"index", 30, "in", false},  {"index", 2, "dex", true},  {"index", 2, "dexx", false},
    {"index", 2, "dexx", false}, {"index", 3, "dex", false}, {"123@#", 0, "123@#", true},
    {"123@#", 2, "3", true},     {"123@#", 2, "@", false},   {"abcdef", 0, "abcdef", true},
    {"", 0, "abcded", false},    {"BA", 0, "ab", false},     {"ab", 0, "ab\n", false},
    {"1222", 0, "12", true},
};
PARAMS_TEST(UtilsTest, ShouldContainsAt, ContainsAtData)
{
    auto &[string, index, search, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::containsAt(string, index, search), expected);
}

Params<std::string_view, size_t, std::vector<std::string_view>, std::optional<std::string_view>> ContainsAtMultitData{
    {"", 0, {""}, std::nullopt},
    {"", 12, {""}, std::nullopt},
    {"i", 0, {"i"}, "i"},
    {"index", 0, {"none", "in"}, "in"},
    {"index", 0, {"oh", "bob", "in"}, "in"},
    {"index", 30, {"in"}, std::nullopt},
    {"index", 2, {"yes", "dex"}, "dex"},
    {"index", 2, {"dexx"}, std::nullopt},
    {"index", 2, {"dexx"}, std::nullopt},
    {"index", 3, {"dex"}, std::nullopt},
    {"123@#", 0, {"123@#"}, "123@#"},
    {"123@#", 2, {"3"}, "3"},
    {"123@#", 2, {"@"}, std::nullopt},
    {"abcdef", 0, {"abcdef"}, "abcdef"},
    {"", 0, {"abcded"}, std::nullopt},
    {"BA", 0, {"ab"}, std::nullopt},
    {"ab", 0, {"ab\n"}, std::nullopt},
    {"1222", 0, {"12"}, "12"},
};
PARAMS_TEST(UtilsTest, ShouldContainsAtMulti, ContainsAtMultitData)
{
    auto &[string, index, searches, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::containsAt(string, index, searches), expected);
}

Params<std::string_view, size_t, std::string_view, bool> ContainsAtFromEndData{
    {"", 0, "", false},         {"", 12, "", false},           {"i", 0, "i", true},
    {"index", 1, "in", true},   {"index", 4, "in", false},     {"index", 30, "in", false},
    {"index", 4, "dex", true},  {"index", 2, "dexx", false},   {"index", 2, "dexx", false},
    {"index", 2, "dex", false}, {"123@#", 4, "123@#", true},   {"123@#", 2, "3", true},
    {"123@#", 2, "@", false},   {"abcdef", 5, "abcdef", true}, {"", 0, "abcded", false},
    {"BA", 2, "ab", false},     {"ab", 2, "ab\n", false},      {"1222", 1, "12", true},
};
PARAMS_TEST(UtilsTest, ShouldContainsAtFromEnd, ContainsAtFromEndData)
{
    auto &[string, index, search, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::containsAtFromEnd(string, index, search), expected);
}

Params<std::string_view, size_t, std::vector<std::string_view>, std::optional<std::string_view>>
    ContainsAtFromEndMultitData{
        {"", 0, {}, std::nullopt},
        {"hello", 0, {}, std::nullopt},
        {"", 0, {""}, std::nullopt},
        {"", 12, {""}, std::nullopt},
        {"i", 0, {"i"}, "i"},
        {"index", 1, {"none", "in"}, "in"},
        {"index", 1, {"oh", "bob", "in"}, "in"},
        {"index", 30, {"in"}, std::nullopt},
        {"index", 4, {"yes", "dex"}, "dex"},
        {"index", 2, {"dexx"}, std::nullopt},
        {"index", 2, {"dexx"}, std::nullopt},
        {"index", 3, {"dex"}, std::nullopt},
        {"123@#", 4, {"123@#"}, "123@#"},
        {"123@#", 2, {"3"}, "3"},
        {"123@#", 2, {"@"}, std::nullopt},
        {"abcdef", 5, {"abcdef"}, "abcdef"},
        {"", 0, {"abcded"}, std::nullopt},
        {"BA", 0, {"ab"}, std::nullopt},
        {"ab", 0, {"ab\n"}, std::nullopt},
        {"1222", 1, {"12"}, "12"},
    };
PARAMS_TEST(UtilsTest, ShouldContainsAtFromEndMulti, ContainsAtFromEndMultitData)
{
    auto &[string, index, searches, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::containsAtFromEnd(string, index, searches), expected);
}

Params<std::string_view, std::string_view, bool> StartsWithData{
    {"", "", true},         {"1234567", "123", true}, {"1234567", "", true},
    {"1234567", "1", true}, {"123", "123", true},     {"123", "1234", false},
    {"123", "890", false},  {"123", "1245", false},   {"1234567", "234", false},
    {"", "234", false},     {"123", "234", false},
};
PARAMS_TEST(UtilsTest, ShouldStartsWith, StartsWithData)
{
    auto &[string, search, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::startsWith(string, search), expected);
}

Params<std::string, std::string, std::vector<std::string_view>> SplitStrData{
    {"", "/", {""}},
    {"", "", {""}},
    {"abcd", "", {"abcd"}},
    {"first:sec:for:5:6", "/", {"first:sec:for:5:6"}},
    {"first:sec:for:5:6", ":", {"first", "sec", "for", "5", "6"}},
    {"first123++/?sec123++/?for123++/?5123++/?6", "123++/?", {"first", "sec", "for", "5", "6"}},
    {":first:sec:for:5:6", ":", {"", "first", "sec", "for", "5", "6"}},
    {"first::sec:for:5:6", ":", {"first", "", "sec", "for", "5", "6"}},
    {"first:sec:for:5:6:", ":", {"first", "sec", "for", "5", "6", ""}},
    {"first__sec__for__5__6__", "__", {"first", "sec", "for", "5", "6", ""}},
    {":::", ":", {"", "", "", ""}},
};
PARAMS_TEST(UtilsTest, ShouldSplitString, SplitStrData)
{
    auto &[string, delim, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::split(string, delim), expected);
}

Params<std::string, std::vector<std::string_view>, std::vector<std::string_view>> SplitStrMultiData{
    {"", {}, {""}},
    {"hello", {}, {"hello"}},
    {"", {"/", "|"}, {""}},
    {"", {"", "|"}, {""}},
    {"abcd", {""}, {"abcd"}},
    {"ab:cd", {"", ":"}, {"ab", "cd"}},
    {"first:sec__for:5||6", {":", "__", "||"}, {"first", "sec", "for", "5", "6"}},
    {"first:sec__for:5||6", {":", "||"}, {"first", "sec__for", "5", "6"}},
    {"first:sec__for:5||6", {"???"}, {"first:sec__for:5||6"}},
    {"first:sec:for:5:6", {":"}, {"first", "sec", "for", "5", "6"}},
    {"first123++/?sec123++/?for123++/?5123++/?6", {"123++/?"}, {"first", "sec", "for", "5", "6"}},
    {":first:sec:for:5:6", {":"}, {"", "first", "sec", "for", "5", "6"}},
    {"first::sec:for:5:6", {":"}, {"first", "", "sec", "for", "5", "6"}},
    {"first:sec:for:5:6:", {":"}, {"first", "sec", "for", "5", "6", ""}},
    {":::", {":"}, {"", "", "", ""}},
};
PARAMS_TEST(UtilsTest, ShouldSplitStringMulti, SplitStrMultiData)
{
    auto &[string, delims, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::split(string, delims), expected);
}

Params<std::string, std::vector<std::string_view>, std::optional<std::array<std::string_view, 2>>> BreakStrData{
    {"", {""}, std::nullopt},
    {"", {}, std::nullopt},
    {"hello", {}, std::nullopt},
    {"key=value", {"="}, std::array<std::string_view, 2>{"key", "value"}},
    {"key====value", {"===="}, std::array<std::string_view, 2>{"key", "value"}},
    {"=keyvalue", {"="}, std::array<std::string_view, 2>{"", "keyvalue"}},
    {"keyvalue=", {"="}, std::array<std::string_view, 2>{"keyvalue", ""}},
    {"key=value", {"-"}, std::nullopt},
    {"abcd", {""}, std::nullopt},
    {"first=sec:for:5:6", {"/"}, std::nullopt},
    {"first:sec:for:5:6", {":"}, std::array<std::string_view, 2>{"first", "sec:for:5:6"}},
    {"first__sec__for__5__6__", {"__"}, std::array<std::string_view, 2>{"first", "sec__for__5__6__"}},
};
PARAMS_TEST(UtilsTest, ShouldBreakString, BreakStrData)
{
    auto &[string, delim, expected] = GetParam();
    EXPECT_EQ(sb::cf::details::utils::tryBreak(string, delim), expected);
}

Params<std::string, std::vector<std::string_view>, std::optional<std::array<std::string_view, 2>>> BreakFromEndStrData{
    {"", {""}, std::nullopt},
    {"", {}, std::nullopt},
    {"hello", {}, std::nullopt},
    {"key=value", {"="}, std::array<std::string_view, 2>{"key", "value"}},
    {"key====value", {"===="}, std::array<std::string_view, 2>{"key", "value"}},
    {"=keyvalue", {"="}, std::array<std::string_view, 2>{"", "keyvalue"}},
    {"keyvalue=", {"="}, std::array<std::string_view, 2>{"keyvalue", ""}},
    {"key=value", {"-"}, std::nullopt},
    {"abcd", {""}, std::nullopt},
    {"first=sec:for:5:6", {"/"}, std::nullopt},
    {"first:sec:for:5:6", {":"}, std::array<std::string_view, 2>{"first:sec:for:5", "6"}},
    {"first__sec__for__5__6", {"__"}, std::array<std::string_view, 2>{"first__sec__for__5", "6"}},
};
PARAMS_TEST(UtilsTest, ShouldBreakFromEndString, BreakFromEndStrData)
{
    auto &[string, delim, expected] = GetParam();
    auto res = sb::cf::details::utils::tryBreakFromEnd(string, delim);
    EXPECT_EQ(sb::cf::details::utils::tryBreakFromEnd(string, delim), expected);
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