#include <gtest/gtest.h>
#include <memory>

#include "Classes/CustomConfigSource.hpp"
#include "SevenBit/Conf/ConfigurationBuilder.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/ObjectHolder.hpp"

class ConfigurationBuilderTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ConfigurationBuilderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(ConfigurationBuilderTest, ShouldFailCreationDueToNullSource)
{
    EXPECT_THROW(sb::cf::ConfigurationBuilder{{nullptr}}, sb::cf::NullPointerException);
}

TEST_F(ConfigurationBuilderTest, ShouldFailAddDueToNullSource)
{
    auto builder = sb::cf::ConfigurationBuilder{};

    EXPECT_THROW(builder.add(nullptr), sb::cf::NullPointerException);
}

TEST_F(ConfigurationBuilderTest, ShouldFailBuildDueToNullSource)
{
    auto builder = sb::cf::ConfigurationBuilder{};

    builder.getSources().emplace_back(nullptr);

    EXPECT_THROW(builder.build(), sb::cf::NullPointerException);
}

TEST_F(ConfigurationBuilderTest, ShouldClearSources)
{
    auto builder = sb::cf::ConfigurationBuilder{};

    builder.addAppSettings("dev");

    EXPECT_FALSE(builder.getSources().empty());

    builder.clear();

    EXPECT_TRUE(builder.getSources().empty());
}

TEST_F(ConfigurationBuilderTest, ShouldBuildSimpleConfig)
{
    auto builder = sb::cf::ConfigurationBuilder{};

    builder.addAppSettings("dev")
        .addJson({{"string", 1}})
        .addCommandLine({"--string=2", "array!json=[3,2,1]"})
        .AddInMemory("set:set", 44444)
        .addKeyPerFile("Directory");

    EXPECT_NO_THROW(builder.build());
}

TEST_F(ConfigurationBuilderTest, ShouldBuildConfigWithProperties)
{
    auto builder = sb::cf::ConfigurationBuilder{};

    builder.getProperties()["counter"] = sb::cf::ObjectHolder<int>::from(0);

    auto conf = builder.addAppSettings("dev")
                    .addJson({{"string", 1}})
                    .addCommandLine({"--string=2", "array=3,2,1"})
                    .AddInMemory("set:set", 44444)
                    .addKeyPerFile("Directory")
                    .add(std::make_unique<CustomConfigSource>())
                    .add(std::make_unique<CustomConfigSource>())
                    .add(std::make_unique<CustomConfigSource>())
                    .add(std::make_unique<CustomConfigSource>())
                    .build();

    const auto cnt = sb::cf::ObjectHolder<int>::safeCastFrom(*builder.getProperties()["counter"]).get();

    EXPECT_EQ(cnt, 4);
}
