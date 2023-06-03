#include <cstddef>
#include <gtest/gtest.h>
#include <iostream>

#include "SevenBit/Conf/Details/Utils.hpp"

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

TEST_F(UtilsTest, ShouldNotSplitString)
{
    auto splitted = sb::cf::utils::split("first:sec:for:5:6", "/");
    EXPECT_EQ(splitted.size(), 1);
    EXPECT_EQ(splitted[0], "first:sec:for:5:6");
}

TEST_F(UtilsTest, ShouldSplitString)
{
    auto splitted = sb::cf::utils::split("first:sec:for:5:6", ":");
    EXPECT_EQ(splitted.size(), 5);
    EXPECT_EQ(splitted[0], "first");
    EXPECT_EQ(splitted[1], "sec");
    EXPECT_EQ(splitted[2], "for");
    EXPECT_EQ(splitted[3], "5");
    EXPECT_EQ(splitted[4], "6");
}

TEST_F(UtilsTest, ShouldSplitComplexString)
{
    auto splitted = sb::cf::utils::split("first123++/?sec123++/?for123++/?5123++/?6", "123++/?");
    EXPECT_EQ(splitted.size(), 5);
    EXPECT_EQ(splitted[0], "first");
    EXPECT_EQ(splitted[1], "sec");
    EXPECT_EQ(splitted[2], "for");
    EXPECT_EQ(splitted[3], "5");
    EXPECT_EQ(splitted[4], "6");
}

TEST_F(UtilsTest, ShouldSplitEmptyBeginString)
{
    auto splitted = sb::cf::utils::split(":first:sec:for:5:6", ":");
    EXPECT_EQ(splitted.size(), 6);
    EXPECT_EQ(splitted[0], "");
    EXPECT_EQ(splitted[1], "first");
    EXPECT_EQ(splitted[2], "sec");
    EXPECT_EQ(splitted[3], "for");
    EXPECT_EQ(splitted[4], "5");
    EXPECT_EQ(splitted[5], "6");
}

TEST_F(UtilsTest, ShouldSplitEmptyEndString)
{
    auto splitted = sb::cf::utils::split("first:sec:for:5:6:", ":");
    EXPECT_EQ(splitted.size(), 6);
    EXPECT_EQ(splitted[0], "first");
    EXPECT_EQ(splitted[1], "sec");
    EXPECT_EQ(splitted[2], "for");
    EXPECT_EQ(splitted[3], "5");
    EXPECT_EQ(splitted[4], "6");
    EXPECT_EQ(splitted[5], "");
}

TEST_F(UtilsTest, ShouldSplitEmptyMidString)
{
    auto splitted = sb::cf::utils::split("first:sec:for::5:6", ":");
    EXPECT_EQ(splitted.size(), 6);
    EXPECT_EQ(splitted[0], "first");
    EXPECT_EQ(splitted[1], "sec");
    EXPECT_EQ(splitted[2], "for");
    EXPECT_EQ(splitted[3], "");
    EXPECT_EQ(splitted[4], "5");
    EXPECT_EQ(splitted[5], "6");
}

TEST_F(UtilsTest, ShouldSplitMax2String)
{
    auto splitted = sb::cf::utils::split("first:sec:for::5:6", ":", 2);
    EXPECT_EQ(splitted.size(), 2);
    EXPECT_EQ(splitted[0], "first");
    EXPECT_EQ(splitted[1], "sec:for::5:6");
}

TEST_F(UtilsTest, ShouldJoinStrings)
{
    auto result = sb::cf::utils::join({"1", "2", "3", "4"}, ":");
    EXPECT_EQ(result, "1:2:3:4");
}

TEST_F(UtilsTest, ShouldJoinSingleString)
{
    auto result = sb::cf::utils::join({"1"}, ":");
    EXPECT_EQ(result, "1");
}

TEST_F(UtilsTest, ShouldJoinEmptyString)
{
    auto result = sb::cf::utils::join({}, ":");
    EXPECT_EQ(result, "");
}

TEST_F(UtilsTest, ShouldStartsWith)
{
    EXPECT_TRUE(sb::cf::utils::startsWith("1234567", "123"));
    EXPECT_TRUE(sb::cf::utils::startsWith("1234567", ""));
    EXPECT_TRUE(sb::cf::utils::startsWith("1234567", "1"));
    EXPECT_TRUE(sb::cf::utils::startsWith("123", "123"));
    EXPECT_FALSE(sb::cf::utils::startsWith("123", "1234"));
    EXPECT_FALSE(sb::cf::utils::startsWith("123", "890"));
    EXPECT_FALSE(sb::cf::utils::startsWith("123", "1245"));
    EXPECT_FALSE(sb::cf::utils::startsWith("1234567", "234"));
    EXPECT_FALSE(sb::cf::utils::startsWith("", "234"));
    EXPECT_FALSE(sb::cf::utils::startsWith("123", "234"));
}

TEST_F(UtilsTest, ShouldEndsWith)
{
    EXPECT_TRUE(sb::cf::utils::endsWith("1234567", "567"));
    EXPECT_TRUE(sb::cf::utils::endsWith("1234567", ""));
    EXPECT_TRUE(sb::cf::utils::endsWith("1234567", "7"));
    EXPECT_TRUE(sb::cf::utils::endsWith("123", "123"));
    EXPECT_FALSE(sb::cf::utils::endsWith("123", "1234"));
    EXPECT_FALSE(sb::cf::utils::endsWith("123", "890"));
    EXPECT_FALSE(sb::cf::utils::endsWith("123", "1245"));
    EXPECT_FALSE(sb::cf::utils::endsWith("1234567", "234"));
    EXPECT_FALSE(sb::cf::utils::endsWith("", "234"));
    EXPECT_FALSE(sb::cf::utils::endsWith("123", "234"));
}

TEST_F(UtilsTest, ShouldReplaceAllStrings)
{
    std::string str = "bup__hup__mub";
    EXPECT_TRUE(sb::cf::utils::replaceAll(str, "__", ":"));
    EXPECT_EQ(str, "bup:hup:mub");
}

TEST_F(UtilsTest, ShouldIgnoreCaseCompareStrings)
{
    EXPECT_TRUE(sb::cf::utils::ignoreCaseEquals("", ""));
    EXPECT_TRUE(sb::cf::utils::ignoreCaseEquals("I", "i"));
    EXPECT_TRUE(sb::cf::utils::ignoreCaseEquals("i", "I"));
    EXPECT_TRUE(sb::cf::utils::ignoreCaseEquals("123@#", "123@#"));
    EXPECT_TRUE(sb::cf::utils::ignoreCaseEquals("abcdef", "ABCDEF"));
    EXPECT_TRUE(sb::cf::utils::ignoreCaseEquals("abcDEF", "abcdef"));
    EXPECT_FALSE(sb::cf::utils::ignoreCaseEquals("abcDEF", "abcded"));
    EXPECT_FALSE(sb::cf::utils::ignoreCaseEquals("", "abcded"));
    EXPECT_FALSE(sb::cf::utils::ignoreCaseEquals("BA", "ab"));
    EXPECT_FALSE(sb::cf::utils::ignoreCaseEquals("ab", "ab\n"));
    EXPECT_FALSE(sb::cf::utils::ignoreCaseEquals("1222", "12"));
}

TEST_F(UtilsTest, ShouldCheckNumberStrings)
{
    EXPECT_TRUE(sb::cf::utils::isNumberString("123"));
    EXPECT_TRUE(sb::cf::utils::isNumberString("1"));
    EXPECT_TRUE(sb::cf::utils::isNumberString("0912837545234123"));
    EXPECT_FALSE(sb::cf::utils::isNumberString("asd"));
    EXPECT_FALSE(sb::cf::utils::isNumberString(""));
    EXPECT_FALSE(sb::cf::utils::isNumberString("alk1"));
    EXPECT_FALSE(sb::cf::utils::isNumberString("1223-"));
    EXPECT_FALSE(sb::cf::utils::isNumberString("1223#"));
    EXPECT_FALSE(sb::cf::utils::isNumberString("1223+="));
    EXPECT_FALSE(sb::cf::utils::isNumberString("1223.123"));
    EXPECT_FALSE(sb::cf::utils::isNumberString("1223.123"));
}

TEST_F(UtilsTest, ShouldCheckToNumber)
{
    EXPECT_EQ(sb::cf::utils::toNumber<int>("123"), std::make_pair(true, 123));
    EXPECT_EQ(sb::cf::utils::toNumber<int>("-123"), std::make_pair(true, -123));
    EXPECT_EQ(sb::cf::utils::toNumber<int>("00123"), std::make_pair(true, 123));
    EXPECT_EQ(sb::cf::utils::toNumber<int>("123 23", false), std::make_pair(true, 123));
    EXPECT_EQ(sb::cf::utils::toNumber<int>("123.23", false), std::make_pair(true, 123));
    EXPECT_EQ(sb::cf::utils::toNumber<int>("123adawadwa", false), std::make_pair(true, 123));
    EXPECT_FALSE(sb::cf::utils::toNumber<size_t>("-123").first);
    EXPECT_FALSE(sb::cf::utils::toNumber<size_t>("").first);
    EXPECT_FALSE(sb::cf::utils::toNumber<size_t>("1223-").first);
    EXPECT_FALSE(sb::cf::utils::toNumber<size_t>("1223#").first);
    EXPECT_FALSE(sb::cf::utils::toNumber<size_t>("1223+=").first);
    EXPECT_FALSE(sb::cf::utils::toNumber<size_t>("1223.123").first);
    EXPECT_FALSE(sb::cf::utils::toNumber<size_t>("1223 123").first);
    EXPECT_FALSE(sb::cf::utils::toNumber<size_t>("asdf").first);
}