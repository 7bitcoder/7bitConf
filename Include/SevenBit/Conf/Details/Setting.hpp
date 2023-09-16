#pragma once

#include <string>
#include <vector>

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    class Setting
    {
      private:
        std::vector<std::string_view> _key;
        JsonValue _value;

      public:
        Setting(std::vector<std::string_view> key, JsonValue value) noexcept
            : _key(std::move(key)), _value(std::move(value))
        {
        }

        Setting(std::string_view key, JsonValue value) noexcept : _key({key}), _value(std::move(value)) {}

        Setting(Setting &&other) noexcept = default;
        Setting(const Setting &other) = default;

        Setting &operator=(Setting &&other) noexcept = default;
        Setting &operator=(const Setting &other) = default;

        bool isValid() const { return !_key.empty(); }

        std::vector<std::string_view> &getKey() { return _key; }

        const std::vector<std::string_view> &getKey() const { return _key; }

        JsonValue &getValue() { return _value; }

        const JsonValue &getValue() const { return _value; }
    };
} // namespace sb::cf::details