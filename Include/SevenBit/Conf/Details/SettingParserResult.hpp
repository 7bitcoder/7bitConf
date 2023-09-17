#pragma once

#include <string>
#include <vector>

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    class SettingParserResult
    {
      private:
        std::vector<std::string_view> _key;
        std::string_view _type;
        std::optional<std::string_view> _value;

      public:
        SettingParserResult() = default;
        SettingParserResult(SettingParserResult &&) = default;
        SettingParserResult(const SettingParserResult &) = default;

        SettingParserResult(std::vector<std::string_view> key, std::string_view type) noexcept
            : _key(std::move(key)), _type(type)
        {
        }

        SettingParserResult(std::string_view key, std::string_view type) noexcept
            : SettingParserResult(std::vector<std::string_view>{key}, type)
        {
        }

        SettingParserResult(std::vector<std::string_view> key, std::string_view type,
                            std::optional<std::string_view> value) noexcept
            : _key(std::move(key)), _type(type), _value(value)
        {
        }

        SettingParserResult(std::string_view key, std::string_view type, std::optional<std::string_view> value) noexcept
            : SettingParserResult(std::vector<std::string_view>{key}, type, value)
        {
        }
        bool isValid() const { return !_key.empty(); }

        const std::vector<std::string_view> &getKey() const { return _key; }

        void setKey(std::vector<std::string_view> key) { _key = key; }

        std::string_view getType() const { return _type; }

        void setType(std::string_view type) { _type = type; }

        bool hasValue() const { return _value.has_value(); }

        std::optional<std::string_view> getValue() const { return _value; }

        void setValue(std::optional<std::string_view> value) { _value = value; }
    };

    inline bool operator==(const SettingParserResult &lhs, const SettingParserResult &rhs) noexcept
    {
        return lhs.getKey() == rhs.getKey() && lhs.getType() == rhs.getType() && lhs.getValue() == rhs.getValue();
    }
} // namespace sb::cf::details