#include <gtest/gtest.h>

#include "Mocks/DeserializerMock.hpp"
#include "Mocks/SettingSplitterMock.hpp"
#include "Mocks/ValueDeserializersMapMock.hpp"
#include "SevenBit/Conf/CommandLineParserBuilder.hpp"
#include "SevenBit/Conf/Details/CommandLineParser.hpp"
#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"

class CommandLineParserBuilderTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CommandLineParserBuilderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(CommandLineParserBuilderTest, ShouldBuildDefault)
{
    const auto parser = sb::cf::CommandLineParserBuilder{}.build();

    const auto &casted = dynamic_cast<sb::cf::details::CommandLineParser &>(*parser);

    auto &splitter = dynamic_cast<const sb::cf::details::SettingSplitter &>(casted.getOptionsSplitter());
    auto &deserializers =
        dynamic_cast<const sb::cf::details::ValueDeserializersMap &>(casted.getValueDeserializersMap());

    EXPECT_EQ(casted.getOptionPrefixes(), (std::vector<std::string_view>{"--", "/"}));
    EXPECT_TRUE(casted.getConsiderSeparated());
    EXPECT_EQ(splitter.getKeySplitters(), std::vector<std::string_view>{":"});
    EXPECT_EQ(splitter.getSettingSplitters(), std::vector<std::string_view>{"="});
    EXPECT_EQ(splitter.getTypeMarkers(), std::vector<std::string_view>{"!"});
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

TEST_F(CommandLineParserBuilderTest, ShouldUseCustomConfig)
{
    sb::cf::CommandLineParserBuilder builder;

    sb::cf::CommandLineParserConfig config;
    config.defaultType = "int";
    config.throwOnUnknownType = false;
    config.allowEmptyKeys = true;
    config.keySplitters = {"/", "++"};
    config.optionSplitters = {"--", "%"};
    config.optionPrefixes = {"@@", "**"};
    config.typeMarkers = {"$", "##"};
    auto parser = builder.useConfig(config).build();

    auto &casted = dynamic_cast<sb::cf::details::CommandLineParser &>(*parser);

    auto &splitter = dynamic_cast<const sb::cf::details::SettingSplitter &>(casted.getOptionsSplitter());
    auto &deserializers =
        dynamic_cast<const sb::cf::details::ValueDeserializersMap &>(casted.getValueDeserializersMap());

    EXPECT_EQ(casted.getOptionPrefixes(), (std::vector<std::string_view>{"@@", "**"}));
    EXPECT_FALSE(casted.getConsiderSeparated());
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

TEST_F(CommandLineParserBuilderTest, ShouldUseValueDeserializer)
{
    sb::cf::CommandLineParserBuilder builder;

    auto parser = builder.useDefaultValueDeserializers()
                      .useValueDeserializer("newType", std::make_unique<DeserializerMock>())
                      .build();

    auto &casted = dynamic_cast<sb::cf::details::CommandLineParser &>(*parser);

    auto &deserializers = casted.getValueDeserializersMap();
    auto get = [&](std::string_view type) { auto &_ = deserializers.getDeserializerFor(type); };
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::SettingSplitter *>(&casted.getOptionsSplitter()));
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::ValueDeserializersMap *>(&casted.getValueDeserializersMap()));
    EXPECT_NO_THROW(get("newType"));
    EXPECT_NO_THROW(get("int"));
    EXPECT_THROW(get("unknown"), sb::cf::ConfigException);
}

TEST_F(CommandLineParserBuilderTest, ShouldUseCustomValueDeserializerMap)
{
    sb::cf::CommandLineParserBuilder builder;

    const auto parser = builder.useValueDeserializersMap(std::make_unique<ValueDeserializersMapMock>()).build();

    const auto &casted = dynamic_cast<sb::cf::details::CommandLineParser &>(*parser);

    EXPECT_TRUE(dynamic_cast<const sb::cf::details::SettingSplitter *>(&casted.getOptionsSplitter()));
    EXPECT_TRUE(dynamic_cast<const ValueDeserializersMapMock *>(&casted.getValueDeserializersMap()));
}

TEST_F(CommandLineParserBuilderTest, ShouldUseCustomSplitter)
{
    sb::cf::CommandLineParserBuilder builder;

    const auto parser = builder.useSplitter(std::make_unique<SettingSplitterMock>()).build();

    const auto &casted = dynamic_cast<sb::cf::details::CommandLineParser &>(*parser);

    EXPECT_TRUE(dynamic_cast<const SettingSplitterMock *>(&casted.getOptionsSplitter()));
    EXPECT_TRUE(dynamic_cast<const sb::cf::details::ValueDeserializersMap *>(&casted.getValueDeserializersMap()));
}
