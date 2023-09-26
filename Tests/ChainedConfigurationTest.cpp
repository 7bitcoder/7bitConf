#include <gtest/gtest.h>
#include <memory>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/JsonConfiguration.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"

class ChainedConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    ChainedConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ChainedConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ChainedConfigurationTest, ShouldFailCreationDueToNullSource)
{
    EXPECT_THROW(sb::cf::ChainedConfigurationSource::create({nullptr}), sb::cf::NullPointerException);
}

TEST_F(ChainedConfigurationTest, ShouldFailAddDueToNullSource)
{
    auto source =
        sb::cf::ChainedConfigurationSource::create({sb::cf::JsonFileConfigurationSource::create("appsettings.json")});

    EXPECT_THROW(source->add(nullptr), sb::cf::NullPointerException);
}

TEST_F(ChainedConfigurationTest, ShouldLoadSimpleChainedConfig)
{
    auto provider =
        sb::cf::ChainedConfigurationSource::create({sb::cf::JsonFileConfigurationSource::create("appsettings.json")})
            ->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"Array", sb::cf::JsonArray{1, 2, 3, 4, 5}},
                                   {"MySetting", "appsettings.json Value"},
                                   {"Logging", {{"LogLevel", {{"Default", "Information"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(ChainedConfigurationTest, ShouldLoadComplexChainedConfig)
{
    auto provider =
        sb::cf::ChainedConfigurationSource::create({sb::cf::JsonFileConfigurationSource::create("appsettings.json"),
                                                    sb::cf::JsonFileConfigurationSource::create("appsettings.dev.json"),
                                                    sb::cf::JsonConfigurationSource::create({{"number", 1}})})
            ->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"Array", sb::cf::JsonArray{11, 2, 3, 4, 5}},
                                   {"number", 1},
                                   {"MySetting", "appsettings.dev.json Value"},
                                   {"ExtraSetting", "extra appsettings.dev.json Value"},
                                   {"Logging", {{"LogLevel", {{"Default", "Warning"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}