#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string_view>

#include "Mocks/DeserializerMock.hpp"
#include "Mocks/SettingSplitterMock.hpp"
#include "Mocks/ValueDeserializersMapMock.hpp"
#include "SevenBit/Conf/Details/CommandLineParser.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

class CommandLineParserTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CommandLineParserTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(CommandLineParserTest, ShouldParseSetting)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::vector<std::string_view> settings = {"--option:deep:deep!string=123", "--option2=123"};
    sb::cf::ISettingSplitter::Result returned1 = {{"option", "deep", "deep"}, "string", "123"};
    sb::cf::ISettingSplitter::Result returned2 = {{"option2"}, std::nullopt, "123"};
    EXPECT_CALL(*splitter, split(std::string_view{"option:deep:deep!string=123"})).WillOnce(testing::Return(returned1));
    EXPECT_CALL(*splitter, split(std::string_view{"option2=123"})).WillOnce(testing::Return(returned2));
    EXPECT_CALL(*deserializers, getDeserializerFor).WillRepeatedly(testing::ReturnRef(deserializer));
    sb::cf::JsonValue returnedValue1 = "123";
    EXPECT_CALL(deserializer, deserialize).WillRepeatedly(testing::Return(returnedValue1));

    sb::cf::details::CommandLineParser parser{std::move(splitter), std::move(deserializers), {"--"}, false};

    EXPECT_EQ(parser.parse(settings),
              (sb::cf::JsonObject{{"option2", "123"}, {"option", {{"deep", {{"deep", "123"}}}}}}));
}

TEST_F(CommandLineParserTest, ShouldParseSeparatedSetting)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::vector<std::string_view> settings = {"--option:deep:deep!int", "123"};
    sb::cf::ISettingSplitter::Result returned1 = {{"option", "deep", "deep"}, "int", std::nullopt};
    EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned1));
    EXPECT_CALL(*deserializers, getDeserializerFor).WillOnce(testing::ReturnRef(deserializer));
    sb::cf::JsonValue returnedValue1 = 123;
    EXPECT_CALL(deserializer, deserialize).WillOnce(testing::Return(returnedValue1));

    sb::cf::details::CommandLineParser parser{std::move(splitter), std::move(deserializers), {"--"}, true};

    EXPECT_EQ(parser.parse(settings), (sb::cf::JsonObject{{"option", {{"deep", {{"deep", 123}}}}}}));
}

TEST_F(CommandLineParserTest, ShouldParseSeparatedEndSetting)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::vector<std::string_view> settings = {"--option:deep:deep!int"};
    sb::cf::ISettingSplitter::Result returned1 = {{"option", "deep", "deep"}, "int", std::nullopt};
    EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned1));
    EXPECT_CALL(*deserializers, getDeserializerFor).WillOnce(testing::ReturnRef(deserializer));
    sb::cf::JsonValue returnedValue1 = 0;
    EXPECT_CALL(deserializer, deserialize).WillOnce(testing::Return(returnedValue1));

    sb::cf::details::CommandLineParser parser{std::move(splitter), std::move(deserializers), {"--"}, true};

    EXPECT_EQ(parser.parse(settings), (sb::cf::JsonObject{{"option", {{"deep", {{"deep", 0}}}}}}));
}

TEST_F(CommandLineParserTest, ShouldFailCreateSettingParserDueNullSplitter)
{
    sb::cf::ISettingSplitter::Ptr splitter;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();

    EXPECT_THROW((sb::cf::details::CommandLineParser{std::move(splitter), std::move(deserializers), {"--"}}),
                 sb::cf::ConfigException);
}

TEST_F(CommandLineParserTest, ShouldFailCreateSettingParserDueNullDeserializers)
{
    sb::cf::IValueDeserializersMap::Ptr deserializers;
    auto splitter = std::make_unique<SettingSplitterMock>();

    EXPECT_THROW((sb::cf::details::CommandLineParser{std::move(splitter), std::move(deserializers), {"--"}}),
                 sb::cf::ConfigException);
}
