#include <gtest/gtest.h>
#include <iostream>
#include <string_view>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

using namespace sb::cf::json;

class JsonObjectExtTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    JsonObjectExtTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~JsonObjectExtTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(JsonObjectExtTest, ShouldFindInner)
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

    EXPECT_EQ(*sb::cf::details::JsonObjectExt::findInner(json, "inner:inner:str"), "hello2");
    EXPECT_FALSE(sb::cf::details::JsonObjectExt::findInner(json, "inner:inner:nonExisting"));
    EXPECT_FALSE(sb::cf::details::JsonObjectExt::findInner(json, "inner:inner:str:nonExisting"));
    EXPECT_FALSE(sb::cf::details::JsonObjectExt::findInner(json, "nonExisting"));
    EXPECT_THROW(sb::cf::details::JsonObjectExt::findInner(json, "inner::inner:str"), sb::cf::ConfigException);
}

TEST_F(JsonObjectExtTest, ShouldInsertInner)
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

    EXPECT_TRUE(sb::cf::details::JsonObjectExt::insertInner(json, "inner:inner:str", "hello3"));
    EXPECT_TRUE(sb::cf::details::JsonObjectExt::insertInner(json, "inner:inner:inner:str", "hello5"));

    sb::cf::JsonObject expectedJson = {{"str", "hello"},
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

TEST_F(JsonObjectExtTest, ShouldFailInsertInner)
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

    EXPECT_ANY_THROW(sb::cf::details::JsonObjectExt::insertInner(json, "inner:inner:str:fail", "hello3"));
}

TEST_F(JsonObjectExtTest, ShouldFailInsertInnerEmpty)
{
    sb::cf::JsonObject json = {{"str", "hello"}};

    EXPECT_ANY_THROW(sb::cf::details::JsonObjectExt::insertInner(json, std::vector<std::string_view>{}, "hello"));
}

TEST_F(JsonObjectExtTest, SouldDeepMerge)
{

    sb::cf::JsonObject json = {{"str", "hello"},
                               {"number", 123},
                               {"inner",
                                {
                                    {"str", "hello1"},
                                    {"number", 1231},

                                }}};

    sb::cf::JsonObject jsonOverride = {{"str", "helloOv"},
                                       {"inner",
                                        {{"number", 12313},
                                         {"inner",
                                          {
                                              {"str", "hello2Ov"},
                                              {"number", 12323},
                                          }}}}};
    sb::cf::details::JsonObjectExt::deepMerge(json, std::move(jsonOverride));

    sb::cf::JsonObject expectedJson = {{"str", "helloOv"},
                                       {"number", 123},
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