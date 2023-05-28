#include <gtest/gtest.h>
#include <iostream>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Json.hpp"

using namespace sb::cf::json;

class JsonObjectExt : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    JsonObjectExt() {}

    void SetUp() override {}

    void TearDown() override {}

    ~JsonObjectExt() {}

    static void TearDownTestSuite() {}
};

TEST_F(JsonObjectExt, ShouldFindInner)
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

    EXPECT_EQ(*sb::cf::JsonObjectExt::findInner(json, "inner:inner:str"), "hello2");
    EXPECT_FALSE(sb::cf::JsonObjectExt::findInner(json, "inner:inner:nonExisting"));
    EXPECT_FALSE(sb::cf::JsonObjectExt::findInner(json, "inner:inner:str:nonExisting"));
    EXPECT_FALSE(sb::cf::JsonObjectExt::findInner(json, "nonExisting"));
}

TEST_F(JsonObjectExt, ShouldInsertInner)
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

    EXPECT_TRUE(sb::cf::JsonObjectExt::insertInner(json, "inner:inner:str", "hello3"));
    EXPECT_TRUE(sb::cf::JsonObjectExt::insertInner(json, "inner:inner:inner:str", "hello5"));

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

TEST_F(JsonObjectExt, SouldDeepMerge)
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
    sb::cf::JsonObjectExt::deepMerge(json, std::move(jsonOverride));

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