#include <gtest/gtest.h>

#include "SevenBit/Conf/ObjectHolder.hpp"

class ObjectHolderTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ObjectHolderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ObjectHolderTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ObjectHolderTest, ShouldCastHolder)
{
    const sb::cf::IObject::Ptr holder = sb::cf::ObjectHolder<int>::from(1);

    auto &casted = sb::cf::ObjectHolder<int>::castFrom(*holder);
    auto &safeCasted = sb::cf::ObjectHolder<int>::safeCastFrom(*holder);

    EXPECT_EQ(casted.get(), 1);
    EXPECT_EQ(&casted, &safeCasted);

    auto act = [&] { auto &_ = sb::cf::ObjectHolder<float>::safeCastFrom(*holder); };
    EXPECT_THROW(act(), std::bad_cast);
}
