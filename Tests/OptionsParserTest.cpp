#include <gtest/gtest.h>
#include <iostream>
#include <tao/json/type.hpp>

#include "SevenBit/Conf/Details/OptionsParser.hpp"
#include "SevenBit/Conf/Json.hpp"

class OptionsParserTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    OptionsParserTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~OptionsParserTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(OptionsParserTest, ShouldParseSimpleOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("--option=hello");

    sb::cf::JsonObject expected = {{"option", "hello"}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseSimpleStringOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("--option!sTrInG=hello");

    sb::cf::JsonObject expected = {{"option", "hello"}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseSimpleBoolOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("--option!bOoL=true");

    sb::cf::JsonObject expected = {{"option", true}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseSimpleIntOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("--option!iNt=1234");

    sb::cf::JsonObject expected = {{"option", 1234}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseSimpleDoubleOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("--option!doUBle=1234.12");

    sb::cf::JsonObject expected = {{"option", 1234.12}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseSimpleJsonOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("--option!jSON={\"yes\": 1234}");

    sb::cf::JsonObject expected = {{"option", {{"yes", 1234}}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseEmptyOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("--=hello");

    sb::cf::JsonObject expected = {{"", "hello"}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseEmptyValueOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option");

    sb::cf::JsonObject expected = {{"option", ""}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseEmptyValueStringOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option!string");

    sb::cf::JsonObject expected = {{"option", ""}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseEmptyValueBoolOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option!bool");

    sb::cf::JsonObject expected = {{"option", false}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseEmptyValueIntOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option!int");

    sb::cf::JsonObject expected = {{"option", 0}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseEmptyValueDoubleOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option!double");

    sb::cf::JsonObject expected = {{"option", 0.0}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseEmptyValuejsonOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option!json");

    sb::cf::JsonObject expected = {{"option", sb::cf::JsonObject{}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseListValueOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option=1,2,3,4,5");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{"1", "2", "3", "4", "5"}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseListValueIntOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option!int=1,2,3,4,5");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{1, 2, 3, 4, 5}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseListValueBoolOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option!Bool=true,false");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{true, false}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseListValueDoubleOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option!Double=1.1,1.4");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{1.1, 1.4}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseListValueStringOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option!String=1.1,1.4");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{"1.1", "1.4"}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseKeyOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option:inner:final=hello");

    sb::cf::JsonObject expected = {{"option", {{"inner", {{{"final", "hello"}}}}}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ShouldParseAlternativeKeyOption)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option__inner__final=hello");

    sb::cf::JsonObject expected = {{"option", {{"inner", {{{"final", "hello"}}}}}}};

    EXPECT_EQ(result, expected);
}