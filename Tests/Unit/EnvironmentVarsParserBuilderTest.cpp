#include <gtest/gtest.h>
#include <string_view>

#include "Mocks/DeserializerMock.hpp"
#include "Mocks/SettingSplitterMock.hpp"
#include "Mocks/ValueDeserializersMapMock.hpp"
#include "SevenBit/Conf/Details/EnvironmentVarsParser.hpp"
#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/EnvironmentVarsParserBuilder.hpp"

class EnvironmentVarsParserBuilderTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    EnvironmentVarsParserBuilderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(EnvironmentVarsParserBuilderTest, ShouldBuildDefault)
{
    const auto parser = sb::cf::EnvironmentVarsParserBuilder{}.build();

    const auto &casted = dynamic_cast<sb::cf::details::EnvironmentVarsParser &>(*parser);

    auto &splitter = dynamic_cast<const sb::cf::details::SettingSplitter &>(casted.getSettingSplitter());
    auto &deserializers =
        dynamic_cast<const sb::cf::details::ValueDeserializersMap &>(casted.getValueDeserializersMap());

    EXPECT_EQ(splitter.getKeySplitters(), (std::vector<std::string_view>{":", "__"}));
    EXPECT_EQ(splitter.getSettingSplitters(), std::vector<std::string_view>{"="});
    EXPECT_EQ(splitter.getTypeMarkers(), (std::vector<std::string_view>{"!", "___"}));
    EXPECT_FALSE(splitter.getAllowEmptyKeys());

    std::vector<std::string_view> types;
    for (auto &[type, _] : deserializers.getDeserializersMap())
    {
        types.push_back(type);
    }
    std::sort(types.begin(), types.end());
    EXPECT_EQ(types, (std::vector<std::string_view>{"bool", "double", "int", "json", "null", "string", "uint"}));
    EXPECT_EQ(deserializers.getDefaultType(), "string");
    EXPECT_TRUE(deserializers.getThrowOnUnknownType());
}

TEST_F(EnvironmentVarsParserBuilderTest, ShouldUseCustomConfig)
{
    sb::cf::EnvironmentVarsParserBuilder builder;

    sb::cf::EnvironmentVarsParserConfig config;
    config.defaultType = "int";
    config.throwOnUnknownType = false;
    config.allowEmptyKeys = true;
    config.keySplitters = {"/", "++"};
    config.variableSplitters = {"--", "%"};
    config.typeMarkers = {"$", "##"};
    auto parser = builder.useConfig(config).build();

    auto &casted = dynamic_cast<sb::cf::details::EnvironmentVarsParser &>(*parser);

    auto &splitter = dynamic_cast<const sb::cf::details::SettingSplitter &>(casted.getSettingSplitter());
    auto &deserializers =
        dynamic_cast<const sb::cf::details::ValueDeserializersMap &>(casted.getValueDeserializersMap());

    EXPECT_EQ(splitter.getKeySplitters(), (std::vector<std::string_view>{"/", "++"}));
    EXPECT_EQ(splitter.getSettingSplitters(), (std::vector<std::string_view>{"--", "%"}));
    EXPECT_EQ(splitter.getTypeMarkers(), (std::vector<std::string_view>{"$", "##"}));
    EXPECT_TRUE(splitter.getAllowEmptyKeys());

    std::vector<std::string_view> types;
    for (auto &[type, _] : deserializers.getDeserializersMap())
    {
        types.push_back(type);
    }
    std::sort(types.begin(), types.end());
    EXPECT_EQ(types, (std::vector<std::string_view>{"bool", "double", "int", "json", "null", "string", "uint"}));
    EXPECT_EQ(deserializers.getDefaultType(), "int");
    EXPECT_FALSE(deserializers.getThrowOnUnknownType());
}

TEST_F(EnvironmentVarsParserBuilderTest, ShouldUseValueDeserializer)
{
    sb::cf::EnvironmentVarsParserBuilder builder;

    auto parser = builder.useDefaultValueDeserializers()
                      .useValueDeserializer("newType", std::make_unique<DeserializerMock>())
                      .build();

    auto &casted = dynamic_cast<sb::cf::details::EnvironmentVarsParser &>(*parser);

    auto &deserializers = casted.getValueDeserializersMap();
    auto get = [&](std::string_view type) { auto &_ = deserializers.getDeserializerFor(type); };
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::SettingSplitter *>(&casted.getSettingSplitter()));
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::ValueDeserializersMap *>(&casted.getValueDeserializersMap()));
    EXPECT_NO_THROW(get("newType"));
    EXPECT_NO_THROW(get("int"));
    EXPECT_THROW(get("unknown"), sb::cf::ConfigException);
}

TEST_F(EnvironmentVarsParserBuilderTest, ShouldUseCustomValueDeserializerMap)
{
    sb::cf::EnvironmentVarsParserBuilder builder;

    const auto parser = builder.useValueDeserializersMap(std::make_unique<ValueDeserializersMapMock>()).build();

    const auto &casted = dynamic_cast<sb::cf::details::EnvironmentVarsParser &>(*parser);

    EXPECT_TRUE(dynamic_cast<const sb::cf::details::SettingSplitter *>(&casted.getSettingSplitter()));
    EXPECT_TRUE(dynamic_cast<const ValueDeserializersMapMock *>(&casted.getValueDeserializersMap()));
}

TEST_F(EnvironmentVarsParserBuilderTest, ShouldUseCustomSplitter)
{
    sb::cf::EnvironmentVarsParserBuilder builder;

    const auto parser = builder.useSplitter(std::make_unique<SettingSplitterMock>()).build();

    const auto &casted = dynamic_cast<sb::cf::details::EnvironmentVarsParser &>(*parser);

    EXPECT_TRUE(dynamic_cast<const SettingSplitterMock *>(&casted.getSettingSplitter()));
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::ValueDeserializersMap *>(&casted.getValueDeserializersMap()));
}
