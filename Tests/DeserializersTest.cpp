#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-param-util.h>
#include <random>
#include <string_view>
#include <tuple>

#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "Utilities/ParamsTest.hpp"

class DeserializersTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    DeserializersTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

static Params<std::string_view, std::optional<std::string_view>, sb::cf::JsonValue> DeserializeData = {
    // Int
    {"int", "12", std::int64_t{12}},
    {"int", "-12", std::int64_t{-12}},
    {"int", std::nullopt, std::int64_t{0}},
    // UInt
    {"uint", "12", std::uint64_t{12}},
    {"uint", "1222", std::uint64_t{1222}},
    {"uint", std::nullopt, std::uint64_t{0}},
    // Bool
    {"bool", "true", true},
    {"bool", "1", true},
    {"bool", "-11", true},
    {"bool", "false", false},
    {"bool", "0", false},
    {"bool", std::nullopt, false},
    // String
    {"string", "hello", std::string{"hello"}},
    {"string", "", std::string{""}},
    {"string", std::nullopt, std::string{""}},
    // Double
    {"double", "1.1", 1.1},
    {"double", "-11.1", -11.1},
    {"double", std::nullopt, 0.0},
    // Null
    {"null", "hello", sb::cf::json::null},
    {"null", std::nullopt, sb::cf::json::null},
    // Json
    {"json", R"({"json": "value"})", sb::cf::JsonObject{{"json", "value"}}},
};
PARAMS_TEST(DeserializersTest, ShouldDeserializeValue, DeserializeData)
{
    const auto &[type, value, expected] = GetParam();
    sb::cf::details::ValueDeserializersMap deserializers;
    deserializers.add("string", std::make_unique<sb::cf::details::StringDeserializer>());
    deserializers.add("bool", std::make_unique<sb::cf::details::BoolDeserializer>());
    deserializers.add("int", std::make_unique<sb::cf::details::IntDeserializer>());
    deserializers.add("double", std::make_unique<sb::cf::details::DoubleDeserializer>());
    deserializers.add("uint", std::make_unique<sb::cf::details::UIntDeserializer>());
    deserializers.add("json", std::make_unique<sb::cf::details::JsonDeserializer>());
    deserializers.add("null", std::make_unique<sb::cf::details::NullDeserializer>());

    auto deserializer = deserializers.getDeserializerFor(type);
    EXPECT_TRUE(deserializer);
    EXPECT_EQ(deserializer->deserialize(value), expected);
}

TEST_F(DeserializersTest, ShouldNotDeserializeValue)
{
    sb::cf::details::ValueDeserializersMap deserializers;
    deserializers.add("string", std::make_unique<sb::cf::details::StringDeserializer>());
    deserializers.add("bool", std::make_unique<sb::cf::details::BoolDeserializer>());
    deserializers.add("int", std::make_unique<sb::cf::details::IntDeserializer>());
    deserializers.add("double", std::make_unique<sb::cf::details::DoubleDeserializer>());
    deserializers.add("uint", std::make_unique<sb::cf::details::UIntDeserializer>());
    deserializers.add("json", std::make_unique<sb::cf::details::JsonDeserializer>());
    deserializers.add("null", std::make_unique<sb::cf::details::NullDeserializer>());

    auto deserializer = deserializers.getDeserializerFor("unknown");
    EXPECT_FALSE(deserializer);
}

static Params<std::string_view, std::optional<std::string_view>> FailDeserializeValues = {
    // Int
    {"int", "123 abcd"},
    {"int", "123 "},
    {"int", "123.123"},
    {"int", " as 123 abcd"},
    {"int", "abcd"},
    {"int", " "},
    {"int", ""},
    {"int", "\n"},
    // UInt
    {"uint", "123 abcd"},
    {"uint", "123 "},
    {"uint", "-12"},
    {"uint", "123.123"},
    {"uint", " as 123 abcd"},
    {"uint", "abcd"},
    {"uint", " "},
    {"uint", ""},
    {"uint", "\n"},
    // Bool
    {"bool", "123 abcd"},
    {"bool", "123.123"},
    {"bool", " as 123 abcd"},
    {"bool", "abcd"},
    {"bool", " "},
    {"bool", ""},
    {"bool", "\n"},
    {"bool", "ttruee"},
    {"bool", "ffalsee"},
    {"bool", "false "},
    {"bool", "false "},
    {"bool", " false"},
    // Double
    {"double", "123 abcd"},
    {"double", "123 "},
    {"double", "123 "},
    {"double", " as 123 abcd"},
    {"double", "abcd"},
    {"double", " "},
    {"double", ""},
    {"double", "\n"},
    // Json
    {"json", "123 abcd"},
    {"json", " as 123 abcd"},
    {"json", "abcd"},
    {"json", "{\"hello: 123}"},
    {"json", "{\"hello\": sde123}"},
    {"json", " "},
    {"json", ""},
    {"json", "\n"},
};
PARAMS_TEST(DeserializersTest, ShouldFailDeserialize, FailDeserializeValues)
{
    const auto &[type, value] = GetParam();
    sb::cf::details::ValueDeserializersMap deserializers;
    deserializers.add("string", std::make_unique<sb::cf::details::StringDeserializer>());
    deserializers.add("bool", std::make_unique<sb::cf::details::BoolDeserializer>());
    deserializers.add("int", std::make_unique<sb::cf::details::IntDeserializer>());
    deserializers.add("double", std::make_unique<sb::cf::details::DoubleDeserializer>());
    deserializers.add("uint", std::make_unique<sb::cf::details::UIntDeserializer>());
    deserializers.add("json", std::make_unique<sb::cf::details::JsonDeserializer>());
    deserializers.add("null", std::make_unique<sb::cf::details::NullDeserializer>());

    auto deserializer = deserializers.getDeserializerFor(type);

    EXPECT_TRUE(deserializer);
    EXPECT_ANY_THROW(deserializer->deserialize(value));
}