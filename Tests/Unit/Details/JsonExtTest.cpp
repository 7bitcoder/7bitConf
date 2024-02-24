#include <gtest/gtest.h>

#include "../../../Include/SevenBit/Conf/Details/JsonExt.hpp"
#include "../../../Include/SevenBit/Conf/Exceptions.hpp"
#include "../../Helpers/Utilities/ParamsTest.hpp"

using namespace sb::cf::json;

class JsonExtTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    JsonExtTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

Params<std::string_view, bool, sb::cf::JsonValue> FindData{
    {"str", true, "hello"},
    {"number", true, 123},
    {"array", true, sb::cf::JsonArray{{{"key", "value"}}}},
    {"inner", true, -123},
    {"nonExisting", false, tao::json::null},
};
PARAMS_TEST(JsonExtTest, ShouldFing, FindData)
{
    sb::cf::JsonValue json = {
        {"str", "hello"}, {"number", 123}, {"array", sb::cf::JsonArray{{{"key", "value"}}}}, {"inner", -123}};

    auto &[keys, expectedFound, expectedValue] = GetParam();
    const auto valuePtr = sb::cf::details::JsonExt::find(json, keys);
    EXPECT_EQ(!!valuePtr, expectedFound);
    if (valuePtr)
    {
        EXPECT_EQ(*valuePtr, expectedValue);
    }
}

Params<std::string_view, bool, sb::cf::JsonValue> DeepFindData{
    {"inner:inner:str", true, "hello2"},
    {"inner:number", true, 1231},
    {"array:0:key", true, "value"},
    {"array:1:key", true, 12},
    {"array:2:key", false, tao::json::null},
    {"array:0.123:key", false, tao::json::null},
    {"array:0 123:key", false, tao::json::null},
    {"array:0 apok:key", false, tao::json::null},
    {"array:-1:key", false, tao::json::null},
    {"array:9notNumber:key", false, tao::json::null},
    {"inner:inner:nonExisting", false, tao::json::null},
    {"inner:inner:str:nonExisting", false, tao::json::null},
    {"nonExisting", false, tao::json::null},
    {"inner::inner:str", false, tao::json::null},

};
PARAMS_TEST(JsonExtTest, ShouldDeepFing, DeepFindData)
{
    sb::cf::JsonValue json = {{"str", "hello"},
                              {"number", 123},
                              {"array", sb::cf::JsonArray{{{"key", "value"}}, {{"key", 12}}}},
                              {"inner",
                               {{"str", "hello1"},
                                {"number", 1231},
                                {"inner",
                                 {
                                     {"str", "hello2"},
                                     {"number", 1232},
                                 }}}}};

    auto &[keys, expectedFound, expectedValue] = GetParam();
    const auto valuePtr = sb::cf::details::JsonExt::deepFind(json, keys);
    EXPECT_EQ(!!valuePtr, expectedFound);
    if (valuePtr)
    {
        EXPECT_EQ(*valuePtr, expectedValue);
    }
}

TEST_F(JsonExtTest, ShouldDeepGetOrOverride)
{
    sb::cf::JsonObject json = {{"str", "hello"},
                               {"number", 123},
                               {"inner",
                                {{"str", "hello1"},
                                 {"number", 1231},
                                 {"inner",
                                  {
                                      {"str", "hello2"},
                                      {"number", 1232},
                                  }}}}};

    sb::cf::details::JsonExt::deepGetOrOverride(json, "inner:inner:str") = "hello3";
    sb::cf::details::JsonExt::deepGetOrOverride(json, "inner:inner:inner:str") = "hello5";

    const sb::cf::JsonObject expectedJson = {{"str", "hello"},
                                             {"number", 123},
                                             {"inner",
                                              {{"str", "hello1"},
                                               {"number", 1231},
                                               {"inner",
                                                {{"str", "hello3"},
                                                 {"number", 1232},
                                                 {"inner",
                                                  {
                                                      {"str", "hello5"},
                                                  }}}}}}};

    EXPECT_EQ(json, expectedJson);
}

TEST_F(JsonExtTest, ShouldDeepGetOrOverrideArrayElement)
{
    sb::cf::JsonObject json = {{"str", "hello"}};

    sb::cf::details::JsonExt::deepGetOrOverride(json, "array:3:object") = "value";

    const sb::cf::JsonObject expected = {
        {"str", "hello"},
        {"array",
         sb::cf::JsonArray{sb::cf::JsonValue{}, sb::cf::JsonValue{}, sb::cf::JsonValue{}, {{"object", "value"}}}},
    };

    EXPECT_EQ(json, expected);
}

TEST_F(JsonExtTest, ShouldDeepGetOrOverrideExistingArrayElement)
{
    sb::cf::JsonObject json = {{"str", "hello"}, {"array", sb::cf::JsonArray{1234, {{"second", "element"}}}}};

    sb::cf::details::JsonExt::deepGetOrOverride(json, "array:3:object") = "value";

    const sb::cf::JsonObject expected = {
        {"str", "hello"},
        {"array", sb::cf::JsonArray{1234, {{"second", "element"}}, sb::cf::JsonValue{}, {{"object", "value"}}}},
    };

    EXPECT_EQ(json, expected);
}

TEST_F(JsonExtTest, ShouldDeepGetOrOverrideWrongArrayElement)
{
    sb::cf::JsonObject json = {{"str", "hello"}};

    sb::cf::details::JsonExt::deepGetOrOverride(json, "array:-3:object") = "value";

    const sb::cf::JsonObject expected = {{"str", "hello"}, {"array", {{"-3", {{"object", "value"}}}}}};

    EXPECT_EQ(json, expected);
}

TEST_F(JsonExtTest, ShouldDeepGetOrOverrideDestroy)
{
    sb::cf::JsonObject json = {{"str", "hello"},
                               {"number", 123},
                               {"inner",
                                {{"str", "hello1"},
                                 {"number", 1231},
                                 {"inner",
                                  {
                                      {"str", "hello2"},
                                      {"number", 1232},
                                  }}}}};

    sb::cf::details::JsonExt::deepGetOrOverride(json, "inner:inner:str:fail") = "value";

    const sb::cf::JsonObject expected = {{"str", "hello"},
                                         {"number", 123},
                                         {"inner",
                                          {{"str", "hello1"},
                                           {"number", 1231},
                                           {"inner",
                                            {
                                                {"str", {{"fail", "value"}}},
                                                {"number", 1232},
                                            }}}}};

    EXPECT_EQ(json, expected);
}

TEST_F(JsonExtTest, ShouldFaildDeepGetOrOverrideEmpty)
{
    sb::cf::JsonObject json = {{"str", "hello"}};

    EXPECT_ANY_THROW(sb::cf::details::JsonExt::deepGetOrOverride(json, std::vector<std::string_view>{}));
}

TEST_F(JsonExtTest, SouldDeepMergeEmptyJsonValue)
{
    sb::cf::JsonValue json;

    sb::cf::JsonValue jsonOverride = {{"str", "helloOv"}};

    sb::cf::details::JsonExt::deepMerge(json, std::move(jsonOverride));

    sb::cf::JsonValue expected = {{"str", "helloOv"}};

    EXPECT_EQ(json, expected);
}

TEST_F(JsonExtTest, SouldDeepMergeJsonValue)
{

    sb::cf::JsonValue json = {{"str", "hello"},
                              {"number", 123},
                              {"array", sb::cf::JsonArray{1, 2, 3, 4, 5, 6}},
                              {"default", "default"},
                              {"inner",
                               {
                                   {"str", "hello1"},
                                   {"number", 1231},

                               }}};

    sb::cf::JsonValue jsonOverride = {{"str", "helloOv"},
                                      {"default", sb::cf::json::uninitialized},
                                      {"array", sb::cf::JsonArray{3, {{"value", "value"}}, 1}},
                                      {"inner",
                                       {{"number", 12313},
                                        {"inner",
                                         {
                                             {"str", "hello2Ov"},
                                             {"number", 12323},
                                         }}}}};

    sb::cf::details::JsonExt::deepMerge(json, std::move(jsonOverride));

    const sb::cf::JsonObject expectedJson = {{"str", "helloOv"},
                                             {"number", 123},
                                             {"array", sb::cf::JsonArray{3, {{"value", "value"}}, 1, 4, 5, 6}},
                                             {"default", "default"},
                                             {"inner",
                                              {{"str", "hello1"},
                                               {"number", 12313},
                                               {"inner",
                                                {
                                                    {"str", "hello2Ov"},
                                                    {"number", 12323},
                                                }}}}};

    EXPECT_EQ(json, expectedJson);
}

TEST_F(JsonExtTest, SouldDeepMergeJsonArray)
{

    sb::cf::JsonArray json{{{"str", "hello"}},
                           {{"number", 123}},
                           sb::cf::JsonValue{},
                           {{"inner",
                             {
                                 {"str", "hello1"},
                                 {"number", 1231},

                             }}}};

    sb::cf::JsonArray jsonOverride{
        {{"str", "hello22"}},
        sb::cf::JsonValue{},
        {{"number", 123}},
    };

    sb::cf::details::JsonExt::deepMerge(json, std::move(jsonOverride));

    const sb::cf::JsonArray expectedJson{{{"str", "hello22"}},
                                         {{"number", 123}},
                                         {{"number", 123}},
                                         {{"inner",
                                           {
                                               {"str", "hello1"},
                                               {"number", 1231},

                                           }}}};

    EXPECT_EQ(json, expectedJson);
}
