#include <gtest/gtest.h>
#include <iostream>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/InMemoryConfiguration.hpp"

class InMemoryConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    InMemoryConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(InMemoryConfigurationTest, ShouldLoadSimpleSettingConfiguration)
{
    auto provider =
        sb::cf::InMemoryConfigurationSource::create({{"yes:yes:inner", sb::cf::JsonArray{1, 2, 3, 4, 5}}})->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"yes", {{"yes", {{"inner", sb::cf::JsonArray{1, 2, 3, 4, 5}}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}
