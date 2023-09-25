#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-param-util.h>
#include <iostream>
#include <string_view>
#include <tuple>

#include "Mocks/DeserializerMock.hpp"
#include "Mocks/SettingSplitterMock.hpp"
#include "Mocks/ValueDeserializersMapMock.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/SettingParserBuilder.hpp"
#include "Utilities/ParamsTest.hpp"

class SettingParserTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    SettingParserTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SettingParserTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(SettingParserTest, ShouldParseSetting)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::string_view setting = "--option:deep:deep!int=123";
    sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, "int", "123"};
    EXPECT_CALL(*splitter, split(setting)).WillOnce(testing::Return(returned));
    EXPECT_CALL(*deserializers, getDeserializerFor(*returned.type)).WillOnce(testing::Return(&deserializer));
    sb::cf::JsonValue returnedValue = 123;
    EXPECT_CALL(deserializer, deserialize(returned.value)).WillOnce(testing::Return(returnedValue));

    sb::cf::details::SettingParser parser{std::move(splitter), std::move(deserializers), "string", false, true};

    EXPECT_EQ(parser.parse(setting), (sb::cf::ISettingParser::Result{{"option", "deep", "deep"}, 123}));
    EXPECT_EQ(parser.getDefaultType(), "string");
    EXPECT_TRUE(parser.getThrowOnUnknownType());
    EXPECT_FALSE(parser.getAllowEmptyKeys());
}

TEST_F(SettingParserTest, ShouldFailCreateSettingParserDueNullSplitter)
{
    sb::cf::ISettingSplitter::Ptr splitter;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();

    EXPECT_THROW((sb::cf::details::SettingParser{std::move(splitter), std::move(deserializers), "string", false, true}),
                 sb::cf::ConfigException);
}

TEST_F(SettingParserTest, ShouldFailCreateSettingParserDueNullDeserializers)
{
    sb::cf::IValueDeserializersMap::Ptr deserializers;
    auto splitter = std::make_unique<SettingSplitterMock>();

    EXPECT_THROW((sb::cf::details::SettingParser{std::move(splitter), std::move(deserializers), "string", false, true}),
                 sb::cf::ConfigException);
}

TEST_F(SettingParserTest, ShouldUseDefaultType)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::string_view setting = "--option:deep:deep=value";
    sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, std::nullopt, "value"};
    EXPECT_CALL(*splitter, split(setting)).WillOnce(testing::Return(returned));
    //    EXPECT_CALL(*deserializers, getDeserializerFor(std::string_view{"string"}))
    //        .WillOnce(testing::Return(&deserializer));
    //    sb::cf::JsonValue returnedValue = "value";
    //    EXPECT_CALL(deserializer, deserialize(returned.value)).WillOnce(testing::Return(returnedValue));
    //
    //    sb::cf::details::SettingParser parser{std::move(splitter), std::move(deserializers), "string", false, true};
    //
    //    EXPECT_EQ(parser.parse(setting), (sb::cf::ISettingParser::Result{{"option", "deep", "deep"}, "value"}));
    //    EXPECT_EQ(parser.getDefaultType(), std::string_view{"string"});
}

TEST_F(SettingParserTest, ShouldNotFailCreateSettingParserDueEmptyKey)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::string_view setting = "--!string=value";
    sb::cf::ISettingSplitter::Result returned = {{""}, "string", "value"};
    EXPECT_CALL(*splitter, split(setting)).WillOnce(testing::Return(returned));
    EXPECT_CALL(*deserializers, getDeserializerFor(*returned.type)).WillOnce(testing::Return(&deserializer));
    sb::cf::JsonValue returnedValue = "value";
    EXPECT_CALL(deserializer, deserialize(returned.value)).WillOnce(testing::Return(returnedValue));

    sb::cf::details::SettingParser parser{std::move(splitter), std::move(deserializers), "string", true, true};

    EXPECT_EQ(parser.parse(setting), (sb::cf::ISettingParser::Result{{""}, "value"}));
    EXPECT_EQ(parser.getDefaultType(), "string");
    EXPECT_TRUE(parser.getThrowOnUnknownType());
    EXPECT_TRUE(parser.getAllowEmptyKeys());
}

TEST_F(SettingParserTest, ShouldFailCreateSettingParserDueEmptyKey)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::string_view setting = "--!string=value";
    sb::cf::ISettingSplitter::Result returned = {{""}, "string", "value"};
    EXPECT_CALL(*splitter, split(setting)).WillOnce(testing::Return(returned));

    sb::cf::details::SettingParser parser{std::move(splitter), std::move(deserializers), "string", false, true};

    EXPECT_THROW(parser.parse(setting), sb::cf::ConfigException);
    EXPECT_EQ(parser.getDefaultType(), "string");
    EXPECT_TRUE(parser.getThrowOnUnknownType());
    EXPECT_FALSE(parser.getAllowEmptyKeys());
}

TEST_F(SettingParserTest, ShouldUseDefaultTypeForUnknownType)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::string_view setting = "--option:deep:deep!unknown=value";
    sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, "unknown", "value"};
    EXPECT_CALL(*splitter, split(setting)).WillOnce(testing::Return(returned));
    EXPECT_CALL(*deserializers, getDeserializerFor(*returned.type)).WillOnce(testing::Return(nullptr));

    EXPECT_CALL(*deserializers, getDeserializerFor(std::string_view{"string"}))
        .WillOnce(testing::Return(&deserializer));
    sb::cf::JsonValue returnedValue = "value";
    EXPECT_CALL(deserializer, deserialize(returned.value)).WillOnce(testing::Return(returnedValue));

    sb::cf::details::SettingParser parser{std::move(splitter), std::move(deserializers), "string", true, false};

    EXPECT_EQ(parser.parse(setting), (sb::cf::ISettingParser::Result{{"option", "deep", "deep"}, "value"}));
    EXPECT_EQ(parser.getDefaultType(), "string");
    EXPECT_FALSE(parser.getThrowOnUnknownType());
    EXPECT_TRUE(parser.getAllowEmptyKeys());
}

TEST_F(SettingParserTest, ShouldFailDueToForUnknownType)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::string_view setting = "--option:deep:deep!unknown=value";
    sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, "unknown", "value"};
    EXPECT_CALL(*splitter, split(setting)).WillOnce(testing::Return(returned));
    EXPECT_CALL(*deserializers, getDeserializerFor(*returned.type)).WillOnce(testing::Return(nullptr));

    sb::cf::details::SettingParser parser{std::move(splitter), std::move(deserializers), "string", true, true};

    EXPECT_THROW(parser.parse(setting), sb::cf::ConfigException);
    EXPECT_EQ(parser.getDefaultType(), "string");
    EXPECT_TRUE(parser.getThrowOnUnknownType());
    EXPECT_TRUE(parser.getAllowEmptyKeys());
}

TEST_F(SettingParserTest, ShouldFailDueToForUnknownDefaultType)
{
    DeserializerMock deserializer;
    auto deserializers = std::make_unique<ValueDeserializersMapMock>();
    auto splitter = std::make_unique<SettingSplitterMock>();

    std::string_view setting = "--option:deep:deep!unknown=value";
    sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, "unknown", "value"};
    EXPECT_CALL(*splitter, split(setting)).WillOnce(testing::Return(returned));
    EXPECT_CALL(*deserializers, getDeserializerFor(*returned.type)).WillOnce(testing::Return(nullptr));

    EXPECT_CALL(*deserializers, getDeserializerFor(std::string_view{"string"})).WillOnce(testing::Return(nullptr));
    sb::cf::JsonValue returnedValue = "value";

    sb::cf::details::SettingParser parser{std::move(splitter), std::move(deserializers), "string", true, false};

    EXPECT_THROW(parser.parse(setting), sb::cf::ConfigException);
    EXPECT_EQ(parser.getDefaultType(), "string");
    EXPECT_FALSE(parser.getThrowOnUnknownType());
    EXPECT_TRUE(parser.getAllowEmptyKeys());
}
