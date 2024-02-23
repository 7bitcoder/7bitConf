#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Mocks/DeserializerMock.hpp"
#include "Mocks/SettingSplitterMock.hpp"
#include "Mocks/ValueDeserializersMapMock.hpp"
#include "SevenBit/Conf/Details/EnvironmentVarsParser.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

class EnvironmentVarsParserTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    EnvironmentVarsParserTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(EnvironmentVarsParserTest, ShouldParseSetting)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::vector<std::string_view> settings = {"--option:deep:deep!int=123"};
    sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, "int", "123"};
    EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned));
    EXPECT_CALL(*deserializers, getDeserializerFor).WillOnce(testing::ReturnRef(deserializer));
    sb::cf::JsonValue returnedValue = 123;
    EXPECT_CALL(deserializer, deserialize).WillOnce(testing::Return(returnedValue));

    sb::cf::details::EnvironmentVarsParser parser{std::move(splitter), std::move(deserializers)};

    EXPECT_EQ(parser.parse(settings), (sb::cf::JsonObject{{"option", {{"deep", {{"deep", 123}}}}}}));
}

TEST_F(EnvironmentVarsParserTest, ShouldFailCreateSettingParserDueNullSplitter)
{
    sb::cf::ISettingSplitter::Ptr splitter;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();

    EXPECT_THROW((sb::cf::details::EnvironmentVarsParser{std::move(splitter), std::move(deserializers)}),
                 sb::cf::ConfigException);
}

TEST_F(EnvironmentVarsParserTest, ShouldFailCreateSettingParserDueNullDeserializers)
{
    sb::cf::IValueDeserializersMap::Ptr deserializers;
    auto splitter = std::make_unique<SettingSplitterMock>();

    EXPECT_THROW((sb::cf::details::EnvironmentVarsParser{std::move(splitter), std::move(deserializers)}),
                 sb::cf::ConfigException);
}
