#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-param-util.h>
#include <random>
#include <string_view>
#include <tuple>

#include "../../../Include/SevenBit/Conf/Details/Deserializers.hpp"
#include "../../../Include/SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "../../Helpers/Utilities/ParamsTest.hpp"

#include <SevenBit/Conf/Details/DefaultDeserializers.hpp>

class ValueDeserializersMapTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ValueDeserializersMapTest() {}

    void SetUp() override {}

    void TearDown() override {}

    static void TearDownTestSuite() {}
};

sb::cf::details::ValueDeserializersMap makeDefaultDeserializersMap(std::string_view defaultType = "string",
                                                                   bool throwOnUnknownType = true)
{
    sb::cf::details::ValueDeserializersMap deserializers{defaultType, throwOnUnknownType};
    sb::cf::details::DefaultDeserializers::add(deserializers);
    return std::move(deserializers);
}

static Params<std::string_view, std::optional<std::string_view>, sb::cf::JsonValue> DeserializeData = {
    // Int
    {"int", "12", std::int64_t{12}},
    {"int", "-12", std::int64_t{-12}},
    {"Int", "-12", std::int64_t{-12}},
    {"INT", "-12", std::int64_t{-12}},
    {"InT", "-12", std::int64_t{-12}},
    {"int", std::nullopt, std::int64_t{0}},
    {"int", "1", std::int64_t{1}},
    {"int", " 1", std::int64_t{1}},
    {"int", "\t\n 1", std::int64_t{1}},
    {"int", "1", std::int64_t{1}},
    {"Int", "1", std::int64_t{1}},
    {"InT", "1", std::int64_t{1}},
    {"INT", "1", std::int64_t{1}},
    {"int", "-1234", std::int64_t{-1234}},
    {"int", "1234", std::int64_t{1234}},
    {"int", "-223372036854775807", std::int64_t{-223372036854775807ll}},
    {"int", "223372036854775807", std::int64_t{223372036854775807ll}},

    // UInt
    {"uint", "12", std::uint64_t{12}},
    {"Uint", "12", std::uint64_t{12}},
    {"UINT", "12", std::uint64_t{12}},
    {"UiNt", "12", std::uint64_t{12}},
    {"uint", "1222", std::uint64_t{1222}},
    {"uint", std::nullopt, std::uint64_t{0}},
    {"uint", "1", std::uint64_t{1}},
    {"uint", " 1", std::uint64_t{1}},
    {"uint", "\t\n 1", std::uint64_t{1}},
    {"uInt", "1", std::uint64_t{1}},
    {"uInT", "1", std::uint64_t{1}},
    {"UINT", "1", std::uint64_t{1}},
    {"uint", "1234", std::uint64_t{1234}},
    {"uint", "223372036854775807", std::uint64_t{223372036854775807ll}},
    {"uint", "17446744073709551615", std::uint64_t{17446744073709551615ull}},

    // Bool
    {"bool", "true", true},
    {"BoOl", "true", true},
    {"bool", "1", true},
    {"bool", "-11", true},
    {"bool", "0", false},
    {"Bool", "true", true},
    {"BOOL", "true", true},
    {"bool", "True", true},
    {"bool", "TrUe", true},
    {"bool", " 1", true},
    {"bool", "\t\n 1", true},
    {"bool", "-1", true},
    {"bool", "-1123", true},
    {"bool", "1123", true},
    {"bool", "false", false},
    {"bool", "False", false},
    {"bool", "FAlSe", false},
    {"bool", std::nullopt, false},
    // String
    {"string", "hello", std::string{"hello"}},
    {"String", "hello", std::string{"hello"}},
    {"STRING", "hello", std::string{"hello"}},
    {"STRING", "hello", std::string{"hello"}},
    {"String", "hello", std::string{"hello"}},
    {"StRinG", "hell\to", std::string{"hell\to"}},
    {"string", "hello", std::string{"hello"}},
    {"string", "", std::string{""}},
    {"string", std::nullopt, std::string{""}},
    // Double
    {"double", "1.1", 1.1},
    {"Double", "1.1", 1.1},
    {"DOUBLE", "1.1", 1.1},
    {"DoUBle", "1.1", 1.1},
    {"double", "-11.1", -11.1},
    {"double", "1", 1.0},
    {"double", " 1", 1.0},
    {"double", "\t\n 1", 1.0},
    {"Double", "1", 1.0},
    {"dOuBle", "1", 1.0},
    {"DOUBLE", "1", 1.0},
    {"double", "1.123", 1.123},
    {"double", "-12.21", -12.21},
    {"double", "10000.11", 10000.11},
    {"double", "-10000.11", -10000.11},
    {"double", std::nullopt, 0.0},
    // Null
    {"null", "hello", sb::cf::json::null},
    {"Null", "hello", sb::cf::json::null},
    {"NULL", "hello", sb::cf::json::null},
    {"nULl", "hello", sb::cf::json::null},
    {"null", "1", sb::cf::json::null},
    {"null", "asdqwdwq", sb::cf::json::null},
    {"Null", "1", sb::cf::json::null},
    {"NULL", "1asdqwdwq", sb::cf::json::null},
    {"NuLl", "1.123 asdqwdwq", sb::cf::json::null},
    {"null", std::nullopt, sb::cf::json::null},
    // Json
    {"json", R"({"json": "value"})", sb::cf::JsonObject{{"json", "value"}}},
    {"Json", R"({"json": "value"})", sb::cf::JsonObject{{"json", "value"}}},
    {"JSON", R"({"json": "value"})", sb::cf::JsonObject{{"json", "value"}}},
    {"jSOn", R"({"json": "value"})", sb::cf::JsonObject{{"json", "value"}}},
    {"JSon", R"({"json": "value"})", sb::cf::JsonObject{{"json", "value"}}},
    {"json", R"({"hello": 123456})", sb::cf::JsonObject{{"hello", 123456}}},
    {"Json", R"({"hello": 123456})", sb::cf::JsonObject{{"hello", 123456}}},
    {"Json", R"({"hello": 123456})", sb::cf::JsonObject{{"hello", 123456}}},
    {"JsOn", R"({"hello": 123456})", sb::cf::JsonObject{{"hello", 123456}}},
    {"json", R"({"hello": [1]})", sb::cf::JsonObject{{"hello", sb::cf::JsonArray{1}}}},
    {"json", R"({"hello": null})", sb::cf::JsonObject{{"hello", sb::cf::json::null}}},
    // Unknown
    {"Unknown", "hello", std::string{"hello"}},
    {"sad", "hello", std::string{"hello"}},
    {"ddd", "hello", std::string{"hello"}},
    {"as", "hello", std::string{"hello"}},
    {"qwed", "hello", std::string{"hello"}},
    {"few", "hell\to", std::string{"hell\to"}},
    {"dda", "hello", std::string{"hello"}},
    {"qwdv", "", std::string{""}},
    {"das", std::nullopt, std::string{""}},
};
PARAMS_TEST(ValueDeserializersMapTest, ShouldDeserializeValue, DeserializeData)
{
    const auto &[type, value, expected] = GetParam();
    auto deserializers = makeDefaultDeserializersMap("string", false);

    auto &deserializer = deserializers.getDeserializerFor(type);
    EXPECT_EQ(deserializer.deserialize(value), expected);
}

TEST_F(ValueDeserializersMapTest, ShouldDeserializeEmptyJsonOption)
{
    auto deserializers = makeDefaultDeserializersMap();

    auto &deserializer = deserializers.getDeserializerFor("json");
    EXPECT_EQ((deserializer.deserialize(std::nullopt)), (sb::cf::JsonValue{}));
}

TEST_F(ValueDeserializersMapTest, ShouldNotFoundDeserializer)
{
    auto deserializers = makeDefaultDeserializersMap();

    deserializers.set("unknown2", nullptr);

    auto get = [&](std::string_view type) { auto &_ = deserializers.getDeserializerFor(type); };
    EXPECT_EQ(deserializers.getDeserializersMap().size(), 8);
    EXPECT_THROW(get("unknown"), sb::cf::ConfigException);
    EXPECT_THROW(get("unknown2"), sb::cf::ConfigException);
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
PARAMS_TEST(ValueDeserializersMapTest, ShouldFailDeserialize, FailDeserializeValues)
{
    const auto &[type, value] = GetParam();
    auto deserializers = makeDefaultDeserializersMap();

    auto &deserializer = deserializers.getDeserializerFor(type);

    EXPECT_ANY_THROW(auto result = deserializer.deserialize(value));
}
