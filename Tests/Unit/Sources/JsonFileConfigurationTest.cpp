#include <gtest/gtest.h>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Sources/JsonFileConfiguration.hpp"

class JsonFileConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    JsonFileConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(JsonFileConfigurationTest, ShouldFailProviderCreationDueToNullSource)
{
    EXPECT_THROW(sb::cf::JsonFileConfigurationProvider(nullptr), sb::cf::NullPointerException);
}

TEST_F(JsonFileConfigurationTest, ShouldLoadSimpleJsonConfigFile)
{
    const auto provider = sb::cf::JsonFileConfigurationSource::create("appsettings.json")->build(mock);

    provider->load();

    const sb::cf::JsonObject expected = {{"Array", sb::cf::JsonArray{1, 2, 3, 4, 5}},
                                         {"MySetting", "appsettings.json Value"},
                                         {"Logging", {{"LogLevel", {{"Default", "Information"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(JsonFileConfigurationTest, ShouldNotLoadNonExistingJsonConfigFile)
{
    const auto provider = sb::cf::JsonFileConfigurationSource::create("nonExisting.json", true)->build(mock);

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}

TEST_F(JsonFileConfigurationTest, ShouldFailLoadingNonExistingJsonConfigFile)
{
    const auto provider = sb::cf::JsonFileConfigurationSource ::create("nonExisting.json")->build(mock);

    EXPECT_THROW(provider->load(), sb::cf::ConfigFileNotFoundException);
}

TEST_F(JsonFileConfigurationTest, ShouldFailLoadingBadJsonConfigFile)
{
    const auto provider = sb::cf::JsonFileConfigurationSource ::create("bad.json")->build(mock);

    EXPECT_THROW(provider->load(), sb::cf::BadConfigFileException);
}
