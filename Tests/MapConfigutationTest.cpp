#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Config/Json.hpp"
#include "SevenBit/Config/JsonConfiguration.hpp"
#include "SevenBit/Config/MapConfiguration.hpp"
#include <gtest/gtest.h>
#include <iostream>

class MapConfigutationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    MapConfigutationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~MapConfigutationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(MapConfigutationTest, ShouldMapSimpleConfiguration)
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
