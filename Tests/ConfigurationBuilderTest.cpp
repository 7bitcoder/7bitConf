#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string_view>

#include "Classes/CustomConfigSource.hpp"
#include "SevenBit/Conf/ConfigurationBuilder.hpp"
#include "SevenBit/Conf/ObjectHolder.hpp"

class ConfigurationBuilderTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ConfigurationBuilderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ConfigurationBuilderTest() {}

    static void TearDownTestSuite() {}
};

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

    auto cnt = sb::cf::ObjectHolder<int>::safeCastFrom(*builder.getProperties()["counter"]).get();

    EXPECT_EQ(cnt, 4);
}
