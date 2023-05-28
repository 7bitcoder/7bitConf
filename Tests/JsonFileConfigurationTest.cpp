#include <gtest/gtest.h>
#include <iostream>

#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"

class JsonFileConfigurationTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    JsonFileConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~JsonFileConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(JsonFileConfigurationTest, SimpleTest)
{
    auto provider = sb::cf::JsonFileConfigurationSource::create("appsettings.json")->build();

    provider->load();

    sb::cf::JsonObject expected = {{"number", 12345},
                                   {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                   {"string", "string"},
                                   {"object", {{"num", 134}, {"string", "string"}}}};
    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(JsonFileConfigurationTest, OptionalNonExistingFileTest)
{
    auto provider = sb::cf::JsonFileConfigurationSource::create("nonExisting.json", true)->build();

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}

TEST_F(JsonFileConfigurationTest, NonExistingFileTest)
{
    auto provider = sb::cf::JsonFileConfigurationSource ::create("nonExisting.json")->build();

    EXPECT_THROW(provider->load(), sb::cf::ConfigFileNotFoundException);
}

TEST_F(JsonFileConfigurationTest, BadFileTest)
{
    auto provider = sb::cf::JsonFileConfigurationSource ::create("bad.json")->build();

    EXPECT_THROW(provider->load(), sb::cf::BadConfigFileException);
}