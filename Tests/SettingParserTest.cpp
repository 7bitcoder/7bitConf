#include <gtest/gtest.h>
#include <iostream>
#include <tao/json/type.hpp>

#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

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

TEST_F(SettingParserTest, ShouldParseSimpleOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("--option=hello");

    sb::cf::JsonObject expected = {{"option", "hello"}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseSimpleStringOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("--option!sTrInG=hello");

    sb::cf::JsonObject expected = {{"option", "hello"}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseSimpleBoolOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("--option!bOoL=true");

    sb::cf::JsonObject expected = {{"option", true}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseSimpleIntOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("--option!iNt=1234");

    sb::cf::JsonObject expected = {{"option", 1234}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseSimpleDoubleOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("--option!doUBle=1234.12");

    sb::cf::JsonObject expected = {{"option", 1234.12}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseSimpleJsonOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("--option!jSON={\"yes\": 1234}");

    sb::cf::JsonObject expected = {{"option", {{"yes", 1234}}}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldFailParseEmptyOption)
{
    sb::cf::details::SettingParser parser;

    EXPECT_THROW(parser.parseSetting("--=hello"), sb::cf::SettingParserException);
    EXPECT_THROW(parser.parseSetting("=hello"), sb::cf::SettingParserException);
    EXPECT_THROW(parser.parseSetting("="), sb::cf::SettingParserException);
    EXPECT_THROW(parser.parseSetting(""), sb::cf::SettingParserException);
}

TEST_F(SettingParserTest, ShouldParseEmptyValueOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option");

    sb::cf::JsonObject expected = {{"option", ""}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseEmptyValueStringOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option!string");

    sb::cf::JsonObject expected = {{"option", ""}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseEmptyValueBoolOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option!bool");

    sb::cf::JsonObject expected = {{"option", false}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseEmptyValueIntOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option!int");

    sb::cf::JsonObject expected = {{"option", 0}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseEmptyValueDoubleOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option!double");

    sb::cf::JsonObject expected = {{"option", 0.0}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseEmptyValuejsonOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option!json");

    sb::cf::JsonObject expected = {{"option", sb::cf::JsonObject{}}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseListValueOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option:2=1");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{sb::cf::JsonValue{}, sb::cf::JsonValue{}, "1"}}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseListValueIntOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option:2!int=1");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{sb::cf::JsonValue{}, sb::cf::JsonValue{}, 1}}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseListValueBoolOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option:2!Bool=true");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{sb::cf::JsonValue{}, sb::cf::JsonValue{}, true}}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseListValueDoubleOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option:2!Double=1.1");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{sb::cf::JsonValue{}, sb::cf::JsonValue{}, 1.1}}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseListValueStringOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option:2!String=1.1");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{sb::cf::JsonValue{}, sb::cf::JsonValue{}, "1.1"}}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseKeyOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option:inner:final=hello");

    sb::cf::JsonObject expected = {{"option", {{"inner", {{{"final", "hello"}}}}}}};

    EXPECT_EQ(result, expected);
}

TEST_F(SettingParserTest, ShouldParseAlternativeKeyOption)
{
    sb::cf::details::SettingParser parser;

    auto result = parser.parseSetting("option__inner__final=hello");

    sb::cf::JsonObject expected = {{"option", {{"inner", {{{"final", "hello"}}}}}}};

    EXPECT_EQ(result, expected);
}