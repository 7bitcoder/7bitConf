#include <gtest/gtest.h>
#include <iostream>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/JsonConfiguration.hpp"
#include "SevenBit/Conf/MapConfiguration.hpp"

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
    auto provider = sb::cf::MapConfigurationSource::create(sb::cf::JsonConfigurationSource::create({{"yes", 12345}}),
                                                           [](sb::cf::JsonObject ob) {
                                                               ob["yes2"] = "yes";
                                                               return std::move(ob);
                                                           })
                        ->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"yes", 12345}, {"yes2", "yes"}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}
