// #include <gmock/gmock.h>
// #include <gtest/gtest.h>
// #include <string_view>
//
// #include "Mocks/DeserializerMock.hpp"
// #include "Mocks/SettingSplitterMock.hpp"
// #include "Mocks/ValueDeserializersMapMock.hpp"
// #include "SevenBit/Conf/Details/EnvironmentVarsParser.hpp"
// #include "SevenBit/Conf/Exceptions.hpp"
//
// class EnvironmentVarsParserTest : public testing::Test
// {
//   protected:
//     static void TearUpTestSuite() {}
//
//     EnvironmentVarsParserTest() {}
//
//     void SetUp() override {}
//
//     void TearDown() override {}
//
//     static void TearDownTestSuite() {}
// };
//
// TEST_F(EnvironmentVarsParserTest, ShouldParseSetting)
// {
//     DeserializerMock deserializer;
//     auto deserializers = std::make_unique<ValueDeserializersMapMock>();
//     auto splitter = std::make_unique<SettingSplitterMock>();
//
//     std::string_view setting = "--option:deep:deep!int=123";
//     sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, "int", "123"};
//     EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned));
//     EXPECT_CALL(*deserializers, getDeserializerFor).WillOnce(testing::Return(&deserializer));
//     sb::cf::JsonValue returnedValue = 123;
//     EXPECT_CALL(deserializer, deserialize).WillOnce(testing::Return(returnedValue));
//
//     sb::cf::details::EnvironmentVarsParser parser{std::move(splitter), std::move(deserializers), "string", false,
//     true};
//
//     EXPECT_EQ(parser.parse(setting), (sb::cf::ISettingParser::Result{{"option", "deep", "deep"}, 123}));
//     EXPECT_EQ(parser.getDefaultType(), "string");
//     EXPECT_TRUE(parser.getThrowOnUnknownType());
//     EXPECT_FALSE(parser.getAllowEmptyKeys());
// }
//
// TEST_F(EnvironmentVarsParserTest, ShouldFailCreateSettingParserDueNullSplitter)
// {
//     sb::cf::ISettingSplitter::Ptr splitter;
//     auto deserializers = std::make_unique<ValueDeserializersMapMock>();
//
//     EXPECT_THROW(
//         (sb::cf::details::EnvironmentVarsParser{std::move(splitter), std::move(deserializers), "string", false,
//         true}), sb::cf::ConfigException);
// }
//
// TEST_F(EnvironmentVarsParserTest, ShouldFailCreateSettingParserDueNullDeserializers)
// {
//     sb::cf::IValueDeserializersMap::Ptr deserializers;
//     auto splitter = std::make_unique<SettingSplitterMock>();
//
//     EXPECT_THROW(
//         (sb::cf::details::EnvironmentVarsParser{std::move(splitter), std::move(deserializers), "string", false,
//         true}), sb::cf::ConfigException);
// }
//
// TEST_F(EnvironmentVarsParserTest, ShouldUseDefaultType)
// {
//     DeserializerMock deserializer;
//     auto deserializers = std::make_unique<ValueDeserializersMapMock>();
//     auto splitter = std::make_unique<SettingSplitterMock>();
//
//     std::string_view setting = "--option:deep:deep=value";
//     sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, std::nullopt, "value"};
//     EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned));
//     EXPECT_CALL(*deserializers, getDeserializerFor).WillOnce(testing::Return(&deserializer));
//     sb::cf::JsonValue returnedValue = "value";
//     EXPECT_CALL(deserializer, deserialize).WillOnce(testing::Return(returnedValue));
//
//     sb::cf::details::EnvironmentVarsParser parser{std::move(splitter), std::move(deserializers), "string", false,
//     true};
//
//     EXPECT_EQ(parser.parse(setting), (sb::cf::ISettingParser::Result{{"option", "deep", "deep"}, "value"}));
//     EXPECT_EQ(parser.getDefaultType(), std::string_view{"string"});
// }
//
// TEST_F(EnvironmentVarsParserTest, ShouldNotFailCreateSettingParserDueEmptyKey)
// {
//     DeserializerMock deserializer;
//     auto deserializers = std::make_unique<ValueDeserializersMapMock>();
//     auto splitter = std::make_unique<SettingSplitterMock>();
//
//     std::string_view setting = "--!string=value";
//     sb::cf::ISettingSplitter::Result returned = {{""}, "string", "value"};
//     EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned));
//     EXPECT_CALL(*deserializers, getDeserializerFor).WillOnce(testing::Return(&deserializer));
//     sb::cf::JsonValue returnedValue = "value";
//     EXPECT_CALL(deserializer, deserialize).WillOnce(testing::Return(returnedValue));
//
//     sb::cf::details::EnvironmentVarsParser parser{std::move(splitter), std::move(deserializers), "string", true,
//     true};
//
//     EXPECT_EQ(parser.parse(setting), (sb::cf::ISettingParser::Result{{""}, "value"}));
//     EXPECT_EQ(parser.getDefaultType(), "string");
//     EXPECT_TRUE(parser.getThrowOnUnknownType());
//     EXPECT_TRUE(parser.getAllowEmptyKeys());
// }
//
// TEST_F(EnvironmentVarsParserTest, ShouldFailCreateSettingParserDueEmptyKey)
// {
//     DeserializerMock deserializer;
//     auto deserializers = std::make_unique<ValueDeserializersMapMock>();
//     auto splitter = std::make_unique<SettingSplitterMock>();
//
//     std::string_view setting = "--!string=value";
//     sb::cf::ISettingSplitter::Result returned = {{""}, "string", "value"};
//     EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned));
//
//     sb::cf::details::EnvironmentVarsParser parser{std::move(splitter), std::move(deserializers), "string", false,
//     true};
//
//     EXPECT_THROW(auto result = parser.parse(setting), sb::cf::ConfigException);
//     EXPECT_EQ(parser.getDefaultType(), "string");
//     EXPECT_TRUE(parser.getThrowOnUnknownType());
//     EXPECT_FALSE(parser.getAllowEmptyKeys());
// }
//
// TEST_F(EnvironmentVarsParserTest, ShouldUseDefaultTypeForUnknownType)
// {
//     DeserializerMock deserializer;
//     auto deserializers = std::make_unique<ValueDeserializersMapMock>();
//     auto splitter = std::make_unique<SettingSplitterMock>();
//
//     std::string_view setting = "--option:deep:deep!unknown=value";
//     sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, "unknown", "value"};
//     EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned));
//     EXPECT_CALL(*deserializers, getDeserializerFor)
//         .WillOnce(testing::Return(nullptr))
//         .WillOnce(testing::Return(&deserializer));
//     sb::cf::JsonValue returnedValue = "value";
//     EXPECT_CALL(deserializer, deserialize).WillOnce(testing::Return(returnedValue));
//
//     sb::cf::details::EnvironmentVarsParser parser{std::move(splitter), std::move(deserializers), "string", true,
//     false};
//
//     EXPECT_EQ(parser.parse(setting), (sb::cf::ISettingParser::Result{{"option", "deep", "deep"}, "value"}));
//     EXPECT_EQ(parser.getDefaultType(), "string");
//     EXPECT_FALSE(parser.getThrowOnUnknownType());
//     EXPECT_TRUE(parser.getAllowEmptyKeys());
// }
//
// TEST_F(EnvironmentVarsParserTest, ShouldFailDueToForUnknownType)
// {
//     DeserializerMock deserializer;
//     auto deserializers = std::make_unique<ValueDeserializersMapMock>();
//     auto splitter = std::make_unique<SettingSplitterMock>();
//
//     std::string_view setting = "--option:deep:deep!unknown=value";
//     sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, "unknown", "value"};
//     EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned));
//     EXPECT_CALL(*deserializers, getDeserializerFor).WillOnce(testing::Return(nullptr));
//
//     sb::cf::details::EnvironmentVarsParser parser{std::move(splitter), std::move(deserializers), "string", true,
//     true};
//
//     EXPECT_THROW(auto result = parser.parse(setting), sb::cf::ConfigException);
//     EXPECT_EQ(parser.getDefaultType(), "string");
//     EXPECT_TRUE(parser.getThrowOnUnknownType());
//     EXPECT_TRUE(parser.getAllowEmptyKeys());
// }
//
// TEST_F(EnvironmentVarsParserTest, ShouldFailDueToForUnknownDefaultType)
// {
//     DeserializerMock deserializer;
//     auto deserializers = std::make_unique<ValueDeserializersMapMock>();
//     auto splitter = std::make_unique<SettingSplitterMock>();
//
//     std::string_view setting = "--option:deep:deep!unknown=value";
//     sb::cf::ISettingSplitter::Result returned = {{"option", "deep", "deep"}, "unknown", "value"};
//     EXPECT_CALL(*splitter, split).WillOnce(testing::Return(returned));
//     EXPECT_CALL(*deserializers, getDeserializerFor).WillRepeatedly(testing::Return(nullptr));
//     sb::cf::JsonValue returnedValue = "value";
//
//     sb::cf::details::EnvironmentVarsParser parser{std::move(splitter), std::move(deserializers), "string", true,
//     false};
//
//     EXPECT_THROW(auto result = parser.parse(setting), sb::cf::ConfigException);
//     EXPECT_EQ(parser.getDefaultType(), "string");
//     EXPECT_FALSE(parser.getThrowOnUnknownType());
//     EXPECT_TRUE(parser.getAllowEmptyKeys());
// }
