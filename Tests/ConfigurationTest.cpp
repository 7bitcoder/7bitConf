#include <gtest/gtest.h>
#include <string_view>

#include "SevenBit/Conf/ConfigurationBuilder.hpp"

class ConfigurationTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ConfigurationTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

TEST_F(ConfigurationTest, ShouldLoadConfig)
{
    auto conf = sb::cf::ConfigurationBuilder{}
                    .addAppSettings("dev")
                    .addJson({{"string", 1}})
                    .addCommandLine({"--string=2", "Array:0!int=33"})
                    .AddInMemory("set:set", 44444)
                    .addKeyPerFile("Directory")
                    .build();

    sb::cf::JsonObject expected = {{"Array", sb::cf::JsonArray{33, 2, 3, 4, 5}},
                                   {"MySetting", "appsettings.dev.json Value"},
                                   {"ExtraSetting", "extra appsettings.dev.json Value"},
                                   {"Logging", {{"LogLevel", {{"Default", "Warning"}}}}},
                                   {"set", {{"set", 44444}}},
                                   {"string", "2"},
                                   {"settingOne",
                                    {{"number", 12345},
                                     {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                                     {"string", "string"},
                                     {"object", {{"num", 134}, {"string", "string"}}}}},
                                   {"settingTwo",
                                    {{"array", sb::cf::JsonArray{1}},
                                     {"string", "stringdev"},
                                     {"object", {{"inner", {{"num", 12345}}}, {"string", "stringdev"}}}}}};

    EXPECT_EQ(conf->root(), expected);
}

TEST_F(ConfigurationTest, ShouldFindConfgValues)
{
    auto conf = sb::cf::ConfigurationBuilder{}
                    .addAppSettings("dev")
                    .addJson({{"string", 1}})
                    .addCommandLine({"--string=2", "Array:0!int=33"})
                    .AddInMemory("set:set", 44444)
                    .addKeyPerFile("Directory")
                    .build();

    EXPECT_TRUE(conf->find("Array"));
    EXPECT_TRUE(conf->find("MySetting"));
    EXPECT_TRUE(conf->deepFind("settingOne:number"));
    EXPECT_TRUE(conf->deepFind({"settingTwo", "array", "0"}));
    EXPECT_TRUE(conf->deepFind("settingTwo:array:0"));
    EXPECT_EQ(conf->at("Array"), (sb::cf::JsonArray{33, 2, 3, 4, 5}));
    EXPECT_EQ(conf->deepAt("settingOne:number"), (std::int64_t{12345}));
    EXPECT_EQ(conf->deepAt("settingTwo:array:0"), (std::int64_t{1}));
    EXPECT_EQ(conf->operator[]("settingTwo:array:0"), (std::int64_t{1}));
    EXPECT_EQ(conf->operator[]({"settingTwo", "array", "0"}), (std::int64_t{1}));
}

TEST_F(ConfigurationTest, ShouldSerializeValues)
{
    auto conf = sb::cf::ConfigurationBuilder{}
                    .addJson({{"string", 1}})
                    .addCommandLine({"--string=2", "Array:0!int=33"})
                    .AddInMemory("set:set", 44444)
                    .build();

    std::string expected = R"({"Array":[33,2,3,4,5],"MySetting":""})";
    std::ostringstream stream;

    EXPECT_EQ(conf->toString(), R"({
 "Array": [
  33
 ],
 "set": {
  "set": 44444
 },
 "string": "2"
})");
    EXPECT_EQ(conf->toString(0, ""), R"({"Array": [33],"set": {"set": 44444},"string": "2"})");
    stream << *conf;
    EXPECT_EQ(stream.str(), R"({"Array":[33],"set":{"set":44444},"string":"2"})");
}
