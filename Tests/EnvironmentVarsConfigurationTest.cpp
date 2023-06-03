#include "SevenBit/Conf/EnvironmentVarsConfiguration.hpp"
#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Json.hpp"
#include <cstdlib>
#include <gtest/gtest.h>
#include <iostream>

#ifdef _WIN32
#define _7BIT_CONF_PUT_ENV _putenv
#else
#define _7BIT_CONF_PUT_ENV putenv
#endif

class EnvironmentVarsConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    EnvironmentVarsConfigurationTest()
    {
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_STRING=test");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_DOUBLE___double=1.4");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_BOOL___bool=true");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_STRING_LIST__0=string");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_STRING_LIST__1=string1");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_STRING_LIST__2=string2");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_NUMBER_LIST__0___int=1");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_NUMBER_LIST__1___int=3");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_NUMBER_LIST__2___int=22");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_JSON___json={\"key\": \"value\"}");
        _7BIT_CONF_PUT_ENV((char *)"7BITCONF_TEST_OBJECT__INNER__OBJECT=string");
        _7BIT_CONF_PUT_ENV((char *)"7BIT_OTHER_CONF_TEST_STRING=string2");
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
                                   {"TEST_DOUBLE", 1.4},
                                   {"TEST_BOOL", true},
                                   {"TEST_NUMBER_LIST", sb::cf::JsonArray{1, 3, 22}},
                                   {"TEST_STRING_LIST", sb::cf::JsonArray{"string", "string1", "string2"}},
                                   {"TEST_OBJECT", {{"INNER", {{"OBJECT", "string"}}}}},
                                   {"TEST_JSON", {{"key", "value"}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(EnvironmentVarsConfigurationTest, ShouldLoadConfFromEnvVarsWithPrefix)
{
    auto provider = sb::cf::EnvironmentVarsConfigurationSource::create("7BIT")->build(mock);

    provider->load();

    sb::cf::JsonObject expected = {{"CONF_TEST_STRING", "test"},
                                   {"CONF_TEST_DOUBLE", 1.4},
                                   {"CONF_TEST_BOOL", true},
                                   {"_OTHER_CONF_TEST_STRING", "string2"},
                                   {"CONF_TEST_NUMBER_LIST", sb::cf::JsonArray{1, 3, 22}},
                                   {"CONF_TEST_STRING_LIST", sb::cf::JsonArray{"string", "string1", "string2"}},
                                   {"CONF_TEST_OBJECT", {{"INNER", {{"OBJECT", "string"}}}}},
                                   {"CONF_TEST_JSON", {{"key", "value"}}}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(EnvironmentVarsConfigurationTest, ShouldNotLoadConfFromEnvVars)
{
    auto provider = sb::cf::EnvironmentVarsConfigurationSource::create("7BITCONFIGURATION_")->build(mock);

    provider->load();

    EXPECT_TRUE(provider->getConfiguration().empty());
}
