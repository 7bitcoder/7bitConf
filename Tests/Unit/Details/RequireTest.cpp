#include <gtest/gtest.h>

#include "SevenBit/Conf/Details/Require.hpp"

class RequireTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    RequireTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~RequireTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(RequireTest, ShouldRequireNotNull)
{
    int test = 123;
    EXPECT_THROW(sb::cf::details::Require::notNull<int>(nullptr), sb::cf::NullPointerException);
    EXPECT_THROW(sb::cf::details::Require::notNull<int>(std::unique_ptr<int>{}), sb::cf::NullPointerException);
    EXPECT_THROW(sb::cf::details::Require::notNull<int>(std::shared_ptr<int>{}), sb::cf::NullPointerException);
    EXPECT_NO_THROW(sb::cf::details::Require::notNull<int>(&test));
    EXPECT_NO_THROW(sb::cf::details::Require::notNull<int>(std::shared_ptr<int>{new int}));
    EXPECT_NO_THROW(sb::cf::details::Require::notNull<int>(std::unique_ptr<int>{new int}));
}
