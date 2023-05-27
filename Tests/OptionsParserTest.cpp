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

TEST_F(OptionsParserTest, SimpleParse)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("--option=hello");

    sb::cf::JsonObject expected = {{"option", "hello"}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, EmptyParse)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("--=hello");

    sb::cf::JsonObject expected = {{"", "hello"}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, UndefinedParse)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option");

    sb::cf::JsonObject expected = {{"option", sb::cf::json::uninitialized}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, ListParse)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option=1;2;3;4;5");

    const sb::cf::JsonValue expected = {{"option", sb::cf::JsonArray{"1", "2", "3", "4", "5"}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, SegmentsParse)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option:inner:final=hello");

    sb::cf::JsonObject expected = {{"option", {{"inner", {{{"final", "hello"}}}}}}};

    EXPECT_EQ(result, expected);
}

TEST_F(OptionsParserTest, SegmentsAlternativeParse)
{
    sb::cf::OptionsParser parser;

    auto result = parser.parseOption("option__inner__final=hello");

    sb::cf::JsonObject expected = {{"option", {{"inner", {{{"final", "hello"}}}}}}};

    EXPECT_EQ(result, expected);
}