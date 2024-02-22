#include <gtest/gtest.h>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Sources/InMemoryConfiguration.hpp"

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

TEST_F(InMemoryConfigurationTest, ShouldFailProviderCreationDueToNullSource)
{
    EXPECT_THROW(sb::cf::InMemoryConfigurationProvider(nullptr), sb::cf::NullPointerException);
}

TEST_F(InMemoryConfigurationTest, ShouldLoadSimpleSettingConfiguration)
{
    const auto provider =
        sb::cf::InMemoryConfigurationSource::create({{"yes:yes:inner", sb::cf::JsonArray{1, 2, 3, 4, 5}}})->build(mock);

    provider->load();

    const sb::cf::JsonObject expected = {{"yes", {{"yes", {{"inner", sb::cf::JsonArray{1, 2, 3, 4, 5}}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}
