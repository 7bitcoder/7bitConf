// #include <gtest/gtest-param-test.h>
// #include <gtest/gtest.h>
// #include <gtest/internal/gtest-param-util.h>
// #include <random>
// #include <string_view>
// #include <tuple>
//
// #include "SevenBit/Conf/Details/Utils.hpp"
// #include "SevenBit/Conf/Details/ValueDeserializers.hpp"
// #include "SevenBit/Conf/Exceptions.hpp"
// #include "Utilities/ParamsTest.hpp"
//
// class DeserializersTest : public testing::Test
//{
//   protected:
//     static void TearUpTestSuite() {}
//
//     DeserializersTest() {}
//
//     void SetUp() override {}
//
//     void TearDown() override {}
//
//     ~DeserializersTest() {}
//
//     static void TearDownTestSuite() {}
// };
//
// static Params<std::vector<std::string_view>, std::string> SettingsValues = {
//     // String
//     {{"option", "nested", "nested"}, "String"},
//     {{"option"}, "StRinG"},
//     {{"option", "nested"}, "str"},
//     {{"option", "nested", "helloooooo", "nun"}, "str"},
//     {{"option", "nested", "long", "nun", "long", "verryLong", "long", "long"}, "str"},
//     {{"option", "3", "nested"}, "json"}};
// PARAMS_TEST_COMBINED_4(SettingKeySplitterTest, ShouldSplitSettings, OptionPrefix, SettingsValues, TypeMarkers,
//                        KeySeparators)
//{
//     const auto &[prefix, params, typeMark, separator] = GetParam();
//     const auto &[key, type] = params;
//     sb::cf::details::SettingKeySplitter splitter{OptionPrefix[1], KeySeparators[0], TypeMarkers[0], KeySeparators[1],
//                                                  TypeMarkers[1]};
//
//     sb::cf::details::SettingKeySplitter::Result expected{key, type};
//
//     auto setting = prefix + sb::cf::details::utils::joinViews(key, separator) + typeMark + type;
//     auto result = splitter.split(setting);
//     EXPECT_EQ(result, expected);
// }
//
// PARAMS_TEST(SettingKeySplitterTest, ShouldSplitSettingsRandom, SettingsValues)
//{
//     const auto &[key, type] = GetParam();
//
//     sb::cf::details::SettingKeySplitter splitter{OptionPrefix[1], KeySeparators[0], TypeMarkers[0], KeySeparators[1],
//                                                  TypeMarkers[1]};
//
//     sb::cf::details::SettingKeySplitter::Result expected{key, type};
//
//     auto keys = randomJoin(key, KeySeparators);
//     auto withOptionPrefix = randomJoin("", keys, OptionPrefix);
//     auto setting = randomJoin(withOptionPrefix, type, TypeMarkers);
//
//     auto result = splitter.split(setting);
//     EXPECT_EQ(result, expected);
// }
//
// static Params<std::string_view, sb::cf::details::SettingKeySplitter::Result> ProblematicSettingsValues = {
//     {"--option!!type", {{"option!"}, "type"}},
//     {"--option!!", {{"option!"}, ""}},
//     {"--option!!:inner!", {{"option!!", "inner"}, ""}},
//     {"--option!!:inner:::!", {{"option!!", "inner", "", "", ""}, ""}},
//     {":option:inner", {{"", "option", "inner"}, ""}},
//     {":option!!:inner", {{"", "option!!", "inner"}, ""}},
//     {":::!", {{"", "", "", ""}, ""}},
//     {"::!:", {{"", "", "!", ""}, ""}},
//     {":", {{"", ""}, ""}},
//     {"__!", {{"", ""}, ""}},
//     {"_______", {{"", "", ""}, ""}},
//     {"__hello_____type", {{"", "hello", ""}, "type"}},
//     {"__hello__type", {{"", "hello", "type"}, ""}},
// };
// PARAMS_TEST(SettingKeySplitterTest, ShouldSplitProblematicSettings, ProblematicSettingsValues)
//{
//     const auto &[setting, expected] = GetParam();
//
//     sb::cf::details::SettingKeySplitter splitter{OptionPrefix[1], KeySeparators[0], TypeMarkers[0], KeySeparators[1],
//                                                  TypeMarkers[1]};
//
//     auto result = splitter.split(setting);
//     EXPECT_EQ(result, expected);
// }
//
// static Params<std::string_view> FailSettingsValues = {"", "--"};
// PARAMS_TEST(SettingKeySplitterTest, ShouldFailSplitSettings, FailSettingsValues)
//{
//     const auto &[setting] = GetParam();
//
//     sb::cf::details::SettingKeySplitter splitter{OptionPrefix[1], KeySeparators[0], TypeMarkers[0], KeySeparators[1],
//                                                  TypeMarkers[1]};
//
//     EXPECT_ANY_THROW(splitter.split(setting));
// }