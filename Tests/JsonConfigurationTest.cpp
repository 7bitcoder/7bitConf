#include <gtest/gtest.h>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/JsonConfiguration.hpp"

class JsonConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    JsonConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(JsonConfigurationTest, ShouldLoadSimpleJsonConfig)
{
    auto provider = sb::cf::JsonConfigurationSource::create({{"hello", 12345}})->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"hello", 12345}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}
