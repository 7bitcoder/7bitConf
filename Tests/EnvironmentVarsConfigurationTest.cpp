#include "SevenBit/Conf/EnvironmentVarsConfiguration.hpp"
#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Json.hpp"
#include <cstdlib>
#include <gtest/gtest.h>
#include <iostream>

class EnvironmentVarsConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    EnvironmentVarsConfigurationTest()
    {
        setenv("7BITCONF_TEST_STRING", "test", 1);
        setenv("7BITCONF_TEST_STRING_LIST", "string,string1,string2", 1);
        setenv("7BITCONF_TEST_OBJECT:INNER:OBJECT", "string", 1);
        setenv("7BIT_OTHER_CONF_TEST_STRING", "string2", 1);
    }

    void SetUp() override {}

    void TearDown() override {}

    ~EnvironmentVarsConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(EnvironmentVarsConfigurationTest, ShouldLoadConfFromEnvVars)
{
    auto provider = sb::cf::EnvironmentVarsConfigurationSource::create("7BITCONF_")->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"TEST_STRING", "test"},
                                   {"TEST_STRING_LIST", sb::cf::JsonArray{"string", "string1", "string2"}},
                                   {"TEST_OBJECT", {{"INNER", {{"OBJECT", "string"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(EnvironmentVarsConfigurationTest, ShouldLoadConfFromEnvVarsWithPrefix)
{
    auto provider = sb::cf::EnvironmentVarsConfigurationSource::create("7BIT")->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"CONF_TEST_STRING", "test"},
                                   {"_OTHER_CONF_TEST_STRING", "string2"},
                                   {"CONF_TEST_STRING_LIST", sb::cf::JsonArray{"string", "string1", "string2"}},
                                   {"CONF_TEST_OBJECT", {{"INNER", {{"OBJECT", "string"}}}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(EnvironmentVarsConfigurationTest, ShouldNotLoadConfFromEnvVars)
{
    auto provider = sb::cf::EnvironmentVarsConfigurationSource::create("7BITCONFIGURATION_")->build(mock);

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}