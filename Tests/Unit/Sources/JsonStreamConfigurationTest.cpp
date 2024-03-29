#include <gtest/gtest.h>
#include <sstream>

#include "Mocks/ConfigurationBuilderMock.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Sources/JsonStreamConfiguration.hpp"

class JsonStreamConfigurationTest : public testing::Test
{
  protected:
    ConfigurationBuilderMock mock;

    static void TearUpTestSuite() {}

    JsonStreamConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(JsonStreamConfigurationTest, ShouldFailProviderCreationDueToNullSource)
{
    EXPECT_THROW(sb::cf::JsonStreamConfigurationProvider(nullptr), sb::cf::NullPointerException);
}

TEST_F(JsonStreamConfigurationTest, ShouldLoadConfigFromStream)
{
    std::stringstream stream;

    stream << R"({"hello": 12345, "string": "asdf"})";

    const auto provider = sb::cf::JsonStreamConfigurationSource::create(stream)->build(mock);

    provider->load();

    const sb::cf::JsonObject expected = {{"hello", 12345}, {"string", "asdf"}};

    EXPECT_EQ(provider->getConfiguration(), expected);
}

TEST_F(JsonStreamConfigurationTest, ShouldFailLoadingConfigFromBadStream)
{
    std::stringstream stream;

    stream << "\"hello\"";

    const auto provider = sb::cf::JsonStreamConfigurationSource::create(stream)->build(mock);

    EXPECT_THROW(provider->load(), sb::cf::BadStreamException);
}

TEST_F(JsonStreamConfigurationTest, ShouldFailLoadingDueToDoubleStreamRead)
{
    std::stringstream stream;

    stream << R"({"hello": 12345, "string": "asdf"})";

    const auto provider = sb::cf::JsonStreamConfigurationSource::create(stream)->build(mock);

    provider->load();

    EXPECT_ANY_THROW(provider->load());
}
