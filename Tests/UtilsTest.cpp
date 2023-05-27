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

TEST_F(UtilsTest, SplitTestNot)
{
    auto splitted = sb::cf::utils::split("first:sec:for:5:6", "/");
    EXPECT_EQ(splitted.size(), 1);
    EXPECT_EQ(splitted[0], "first:sec:for:5:6");
}

TEST_F(UtilsTest, SplitTestSimple)
{
    auto splitted = sb::cf::utils::split("first:sec:for:5:6", ":");
    EXPECT_EQ(splitted.size(), 5);
    EXPECT_EQ(splitted[0], "first");
    EXPECT_EQ(splitted[1], "sec");
    EXPECT_EQ(splitted[2], "for");
    EXPECT_EQ(splitted[3], "5");
    EXPECT_EQ(splitted[4], "6");
}

TEST_F(UtilsTest, SplitTestComplex)
{
    auto splitted = sb::cf::utils::split("first123++/?sec123++/?for123++/?5123++/?6", "123++/?");
    EXPECT_EQ(splitted.size(), 5);
    EXPECT_EQ(splitted[0], "first");
    EXPECT_EQ(splitted[1], "sec");
    EXPECT_EQ(splitted[2], "for");
    EXPECT_EQ(splitted[3], "5");
    EXPECT_EQ(splitted[4], "6");
}

TEST_F(UtilsTest, SplitTestBegin)
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

TEST_F(UtilsTest, SplitTestEnd)
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

TEST_F(UtilsTest, SplitTestMid)
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

TEST_F(UtilsTest, SplitTestMax2)
{
    auto splitted = sb::cf::utils::split("first:sec:for::5:6", ":", 2);
    EXPECT_EQ(splitted.size(), 2);
    EXPECT_EQ(splitted[0], "first");
    EXPECT_EQ(splitted[1], "sec:for::5:6");
}

TEST_F(UtilsTest, JoinTestSimple)
{
    auto result = sb::cf::utils::join({"1", "2", "3", "4"}, ":");
    EXPECT_EQ(result, "1:2:3:4");
}

TEST_F(UtilsTest, JoinSingle)
{
    auto result = sb::cf::utils::join({"1"}, ":");
    EXPECT_EQ(result, "1");
}

TEST_F(UtilsTest, JoinEmpty)
{
    auto result = sb::cf::utils::join({}, ":");
    EXPECT_EQ(result, "");
}

TEST_F(UtilsTest, StartsWith)
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

TEST_F(UtilsTest, EndsWith)
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

TEST_F(UtilsTest, ReplaceAll)
{
    std::string str = "bup__hup__mub";
    EXPECT_TRUE(sb::cf::utils::replaceAll(str, "__", ":"));
    EXPECT_EQ(str, "bup:hup:mub");
}