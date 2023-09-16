#pragma once
#include <cstddef>
#include <cstdint>
#include <tao/json/from_string.hpp>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(JsonTransformersMap transformers, SettingParserConfig cfg)
        : _transformers(std::move(transformers)), _config(cfg)
    {
    }

    INLINE JsonObject SettingParser::parseSetting(std::string_view setting) const
    {
        auto keyValue = details::utils::split(setting, _config.settingSplitter, 2);
        switch (keyValue.size())
        {
        case 1:
            return parseSetting(setting, std::nullopt);
        case 2:
            return parseSetting(keyValue[0], std::make_optional(keyValue[1]));
        default:
            throw SettingParserException("Wrong setting format: " + std::string{setting} +
                                         " it should follow this scheme --<name>=<value>");
        }
    }

    INLINE JsonObject SettingParser::parseSetting(std::string_view key, std::optional<std::string_view> value) const
    {
        try
        {
            auto &transformer = getTransformer(key);
            auto keyStr = sanitizeKey(key);
            auto keys = parseKey(keyStr);
            return parseSetting(std::move(keys), transformer.transform(value));
        }
        catch (std::exception &e)
        {
            throw SettingParserException{"Error for setting: '" + std::string{key} + "', value: '" +
                                         std::string{value ? *value : ""} + "' error: " + e.what()};
        }
    }

    INLINE JsonObject SettingParser::parseSetting(const std::vector<std::string_view> &key, JsonValue value) const
    {
        JsonObject result{};
        JsonObjectExt::getOrCreateInner(result, key) = std::move(value);
        return result;
    }

    INLINE std::string SettingParser::sanitizeKey(std::string_view key) const
    {
        std::string str{key};
        details::utils::replaceAll(str, _config.alternativeKeySplitter, _config.keySplitter);
        return str;
    }

    INLINE std::vector<std::string_view> SettingParser::parseKey(std::string_view key) const
    {
        if (details::utils::startsWith(key, _config.settingPrefix))
        {
            key.remove_prefix(_config.settingPrefix.size());
        }
        if (key.empty())
        {
            throw SettingParserException{"Key cannot be empty"};
        }
        return details::utils::split(key, _config.keySplitter);
    }

    INLINE IJsonTypeTransformer &SettingParser::getTransformer(std::string_view &key) const
    {
        for (auto &[typeStr, transformer] : _transformers)
        {
            if (tryExtractType(key, typeStr))
            {
                return *transformer;
            }
        }
        return *_transformers.front();
    }

    INLINE bool SettingParser::tryExtractType(std::string_view &value, std::string_view typeStr) const
    {
        if (details::utils::ignoreCaseEndsWith(value, typeStr))
        {
            auto original = value;
            value.remove_suffix(typeStr.size());
            if (tryExtractTypeMarker(value, _config.typeMarker))
            {
                return true;
            }
            if (tryExtractTypeMarker(value, _config.alternativeTypeMarker))
            {
                return true;
            }
            value = original;
        }
        return false;
    }

    INLINE bool SettingParser::tryExtractTypeMarker(std::string_view &value, std::string_view typeMarker) const
    {
        if (details::utils::endsWith(value, typeMarker))
        {
            value.remove_suffix(typeMarker.size());
            return true;
        }
        return false;
    }
} // namespace sb::cf::details