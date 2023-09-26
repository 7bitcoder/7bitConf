#include <gtest/gtest.h>
#include <string_view>

#include "Mocks/DeserializerMock.hpp"
#include "Mocks/SettingSplitterMock.hpp"
#include "Mocks/ValueDeserializersMapMock.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/SettingParserBuilder.hpp"

class SettingParserBuilderTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    SettingParserBuilderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SettingParserBuilderTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(SettingParserBuilderTest, ShouldBuildDefault)
{
    sb::cf::SettingParserBuilder builder;

    auto parser = builder.build();

    auto &casted = dynamic_cast<sb::cf::details::SettingParser &>(*parser);

    EXPECT_TRUE(dynamic_cast<const sb::cf::details::SettingSplitter *>(&casted.getSettingSplitter()));
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::ValueDeserializersMap *>(&casted.getValueDeserializersMap()));
    EXPECT_EQ(casted.getDefaultType(), "string");
    EXPECT_FALSE(casted.getAllowEmptyKeys());
    EXPECT_TRUE(casted.getThrowOnUnknownType());
}

TEST_F(SettingParserBuilderTest, ShouldUseCustomConfig)
{
    sb::cf::SettingParserBuilder builder;

    sb::cf::SettingParserConfig config;
    config.defaultType = "int";
    config.throwOnUnknownType = false;
    config.allowEmptyKeys = true;
    auto parser = builder.useConfig(config).build();

    auto &casted = dynamic_cast<sb::cf::details::SettingParser &>(*parser);

    EXPECT_TRUE(dynamic_cast<const sb::cf::details::SettingSplitter *>(&casted.getSettingSplitter()));
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::ValueDeserializersMap *>(&casted.getValueDeserializersMap()));
    EXPECT_EQ(casted.getDefaultType(), "int");
    EXPECT_TRUE(casted.getAllowEmptyKeys());
    EXPECT_FALSE(casted.getThrowOnUnknownType());
}

TEST_F(SettingParserBuilderTest, ShouldUseValueDeserializer)
{
    sb::cf::SettingParserBuilder builder;

    auto parser = builder.useDefaultValueDeserializers()
                      .useValueDeserializer("newType", std::make_unique<DeserializerMock>())
                      .build();

    auto &casted = dynamic_cast<sb::cf::details::SettingParser &>(*parser);

    EXPECT_TRUE(dynamic_cast<const sb::cf::details::SettingSplitter *>(&casted.getSettingSplitter()));
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::ValueDeserializersMap *>(&casted.getValueDeserializersMap()));
    EXPECT_EQ(casted.getDefaultType(), "string");
    EXPECT_FALSE(casted.getAllowEmptyKeys());
    EXPECT_TRUE(casted.getThrowOnUnknownType());
    EXPECT_TRUE(casted.getValueDeserializersMap().getDeserializerFor("newType"));
    EXPECT_TRUE(casted.getValueDeserializersMap().getDeserializerFor("int"));
    EXPECT_TRUE(casted.getValueDeserializersMap().getDeserializerFor("bool"));
}

TEST_F(SettingParserBuilderTest, ShouldUseCustomValueDeserializerMap)
{
    sb::cf::SettingParserBuilder builder;

    auto parser = builder.useValueDeserializersMap(std::make_unique<ValueDeserializersMapMock>()).build();

    auto &casted = dynamic_cast<sb::cf::details::SettingParser &>(*parser);

    EXPECT_TRUE(dynamic_cast<const sb::cf::details::SettingSplitter *>(&casted.getSettingSplitter()));
    EXPECT_TRUE(dynamic_cast<const ValueDeserializersMapMock *>(&casted.getValueDeserializersMap()));
    EXPECT_EQ(casted.getDefaultType(), "string");
    EXPECT_FALSE(casted.getAllowEmptyKeys());
    EXPECT_TRUE(casted.getThrowOnUnknownType());
}

TEST_F(SettingParserBuilderTest, ShouldUseCustomSplitter)
{
    sb::cf::SettingParserBuilder builder;

    auto parser = builder.useSplitter(std::make_unique<SettingSplitterMock>()).build();

    auto &casted = dynamic_cast<sb::cf::details::SettingParser &>(*parser);

    EXPECT_TRUE(dynamic_cast<const SettingSplitterMock *>(&casted.getSettingSplitter()));
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::ValueDeserializersMap *>(&casted.getValueDeserializersMap()));
    EXPECT_EQ(casted.getDefaultType(), "string");
    EXPECT_FALSE(casted.getAllowEmptyKeys());
    EXPECT_TRUE(casted.getThrowOnUnknownType());
}