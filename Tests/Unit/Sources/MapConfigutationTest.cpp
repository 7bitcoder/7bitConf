#include <gtest/gtest.h>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Sources/JsonConfiguration.hpp"
#include "SevenBit/Conf/Sources/MapConfiguration.hpp"

class MapConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    MapConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(MapConfigurationTest, ShouldFailProviderCreationDueToNullSource)
{
    EXPECT_THROW(sb::cf::MapConfigurationProvider(nullptr, nullptr), sb::cf::NullPointerException);
}

TEST_F(MapConfigurationTest, ShouldMapSimpleConfiguration)
{
    const auto provider =
        sb::cf::MapConfigurationSource::create(sb::cf::JsonConfigurationSource::create({{"yes", 12345}}),
                                               [](sb::cf::JsonObject ob) {
                                                   ob["yes2"] = "yes";
                                                   return std::move(ob);
                                               })
            ->build(mock);

    provider->load();

    const sb::cf::JsonObject expected = {{"yes", 12345}, {"yes2", "yes"}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}
