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

    ~DeserializersTest() {}

    static void TearDownTestSuite() {}
};

static Params<std::string_view, std::optional<std::string_view>, sb::cf::JsonValue> DeserializeData = {
    {"int", "12", std::int64_t{12}},
    {"int", "-12", std::int64_t{-12}},
    {"int", std::nullopt, std::int64_t{0}},
    {"uint", "12", std::uint64_t{12}},
    {"uint", std::nullopt, std::uint64_t{0}},
    {"bool", "true", true},
    {"bool", "1", true},
    {"bool", "-11", true},
    {"bool", "false", false},
    {"bool", "0", false},
    {"bool", std::nullopt, false},
    {"string", std::nullopt, std::string{""}},
    {"string", "hello", std::string{"hello"}},
    {"string", "", std::string{""}},
    {"double", "1.1", 1.1},
    {"double", "-11.1", -11.1},
    {"double", std::nullopt, 0.0},
    {"null", std::nullopt, sb::cf::json::null},
    {"null", "hello", sb::cf::json::null},
    {"json", "{\"json\": \"value\"}", sb::cf::JsonObject{{"json", "value"}}},
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