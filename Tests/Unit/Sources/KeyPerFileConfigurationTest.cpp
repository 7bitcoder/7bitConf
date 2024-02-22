#include <gtest/gtest.h>
#include <string>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Sources/KeyPerFileConfiguration.hpp"

class KeyPerFileConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    KeyPerFileConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(KeyPerFileConfigurationTest, ShouldFailLoadingNonExistingDirectory)
{
    const auto source = sb::cf::KeyPerFileConfigurationSource::create("nonexisting");

    EXPECT_ANY_THROW(source->build(mock));
}

TEST_F(KeyPerFileConfigurationTest, ShouldLoadNonExistingDirectory)
{
    const auto provider = sb::cf::KeyPerFileConfigurationSource::create("nonexisting", true)->build(mock);

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}

TEST_F(KeyPerFileConfigurationTest, ShouldLoadDirectoryConfig)
{
    const auto provider = sb::cf::KeyPerFileConfigurationSource::create("Directory")->build(mock);

    provider->load();

    const sb::cf::JsonObject expected = {{"settingOne",
                                          {{"number", 12345},
                                           {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                           {"string", "string"},
                                           {"object", {{"num", 134}, {"string", "string"}}}}},
                                         {"settingTwo",
                                          {{"array", sb::cf::JsonArray{1}},
                                           {"string", "dev"},
                                           {"object", {{"inner", {{"num", 12345}}}, {"string", "dev"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(KeyPerFileConfigurationTest, ShloudLoadFilteredConfigFiles)
{
    const auto provider = sb::cf::KeyPerFileConfigurationSource::create("Directory", false, "settingOne")->build(mock);

    provider->load();

    const sb::cf::JsonObject expected = {{"settingTwo",
                                          {{"array", sb::cf::JsonArray{1}},
                                           {"string", "dev"},
                                           {"object", {{"inner", {{"num", 12345}}}, {"string", "dev"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(KeyPerFileConfigurationTest, ShloudLoadFilteredConditionConfigFiles)
{
    const auto provider =
        sb::cf::KeyPerFileConfigurationSource::create("Directory", false, [](const std::filesystem::path &path) {
            return path.filename() == "settingOne.json";
        })->build(mock);

    provider->load();

    const sb::cf::JsonObject expected = {{"settingTwo",
                                          {{"array", sb::cf::JsonArray{1}},
                                           {"string", "dev"},
                                           {"object", {{"inner", {{"num", 12345}}}, {"string", "dev"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}
