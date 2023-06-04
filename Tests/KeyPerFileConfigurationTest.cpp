#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Config/JsonFileConfiguration.hpp"
#include "SevenBit/Config/KeyPerFileConfiguration.hpp"

class KeyPerFileConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    KeyPerFileConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~KeyPerFileConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(KeyPerFileConfigurationTest, ShouldFailLoadingNonExistingDirectory)
{
    auto source = sb::cf::KeyPerFileConfigurationSource::create("nonexisting");

    EXPECT_ANY_THROW(source->build(mock));
}

TEST_F(KeyPerFileConfigurationTest, ShouldLoadNonExistingDirectory)
{
    auto provider = sb::cf::KeyPerFileConfigurationSource::create("nonexisting", true)->build(mock);

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}

TEST_F(KeyPerFileConfigurationTest, ShouldLoadDirectoryConfig)
{
    auto provider = sb::cf::KeyPerFileConfigurationSource::create("Directory")->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"settingOne",
                                    {{"number", 12345},
                                     {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                     {"string", "string"},
                                     {"object", {{"num", 134}, {"string", "string"}}}}},
                                   {"settingTwo",
                                    {{"array", sb::cf::JsonArray{1}},
                                     {"string", "stringdev"},
                                     {"object", {{"inner", {{"num", 12345}}}, {"string", "stringdev"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(KeyPerFileConfigurationTest, ShloudLoadFilteredConfigFiles)
{
    auto provider = sb::cf::KeyPerFileConfigurationSource::create("Directory", false, "settingOne")->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"settingTwo",
                                    {{"array", sb::cf::JsonArray{1}},
                                     {"string", "stringdev"},
                                     {"object", {{"inner", {{"num", 12345}}}, {"string", "stringdev"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(KeyPerFileConfigurationTest, ShloudLoadFilteredConditionConfigFiles)
{
    auto provider =
        sb::cf::KeyPerFileConfigurationSource::create("Directory", false, [](const std::filesystem::path &path) {
            return path.filename() == "settingOne.json";
        })->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"settingTwo",
                                    {{"array", sb::cf::JsonArray{1}},
                                     {"string", "stringdev"},
                                     {"object", {{"inner", {{"num", 12345}}}, {"string", "stringdev"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}