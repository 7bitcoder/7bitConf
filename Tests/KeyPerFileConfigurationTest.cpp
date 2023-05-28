#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "SevenBit/Conf/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/KeyPerConfiguration.hpp"
#include "SevenBit/Conf/KeyPerFileConfiguration.hpp"

class KeyPerFileConfigurationTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    KeyPerFileConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~KeyPerFileConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(KeyPerFileConfigurationTest, ShouldFailLoadingNonExistingDirectory)
{
    auto provider = sb::cf::KeyPerFileConfigurationSource::create("nonexisting")->build();

    EXPECT_ANY_THROW(provider->load());
}

TEST_F(KeyPerFileConfigurationTest, ShouldLoadNonExistingDirectory)
{
    auto provider = sb::cf::KeyPerFileConfigurationSource::create("nonexisting", true)->build();

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}

TEST_F(KeyPerFileConfigurationTest, ShouldLoadDirectoryConfig)
{
    auto provider = sb::cf::KeyPerFileConfigurationSource::create("Directory")->build();

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
    auto provider = sb::cf::KeyPerFileConfigurationSource::create("Directory", false, "settingOne")->build();

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
        })->build();

    provider->load();

    sb::cf::JsonObject expected = {{"settingTwo",
                                    {{"array", sb::cf::JsonArray{1}},
                                     {"string", "stringdev"},
                                     {"object", {{"inner", {{"num", 12345}}}, {"string", "stringdev"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}