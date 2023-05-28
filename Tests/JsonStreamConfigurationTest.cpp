#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/JsonStreamConfiguration.hpp"

class JsonStreamConfigurationTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    JsonStreamConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~JsonStreamConfigurationTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(JsonStreamConfigurationTest, ShouldLoadConfigFromStream)
{
    std::stringstream stream;

    stream << "{\"hello\": 12345, \"string\": \"asdf\"}";

    auto provider = sb::cf::JsonStreamConfigurationSource::create(stream)->build();

    provider->load();

    sb::cf::JsonObject expected = {{"hello", 12345}, {"string", "asdf"}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(JsonStreamConfigurationTest, ShouldFailLoadingConfigFromBadStream)
{
    std::stringstream stream;

    stream << "\"hello\"";

    auto provider = sb::cf::JsonStreamConfigurationSource::create(stream)->build();

    EXPECT_THROW(provider->load(), sb::cf::BadStreamException);
}

TEST_F(JsonStreamConfigurationTest, ShouldFailLoadingDueToDoubleStreamRead)
{
    std::stringstream stream;

    stream << "{\"hello\": 12345, \"string\": \"asdf\"}";

    auto provider = sb::cf::JsonStreamConfigurationSource::create(stream)->build();

    provider->load();

    EXPECT_ANY_THROW(provider->load());
}