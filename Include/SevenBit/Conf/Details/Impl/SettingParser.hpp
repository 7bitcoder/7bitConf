#pragma once
#include <cstddef>
#include <cstdint>
#include <tao/json/from_string.hpp>

#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(JsonTransformersLookup transformers, SettingParserConfig cfg)
        : _transformers(std::move(transformers)), _config(cfg)
    {
    }

    INLINE Setting SettingParser::parse(std::string_view setting) const
    {
        auto keyValue = details::utils::split(setting, _config.settingSplitter, 2);
        switch (keyValue.size())
        {
        case 1:
            return parse(setting, std::nullopt);
        case 2:
            return parse(keyValue[0], std::make_optional(keyValue[1]));
        default:
            throw SettingParserException("Wrong setting format: " + std::string{setting} +
                                         " it should follow this scheme --<name>=<value>");
        }
    }

    INLINE Setting SettingParser::parse(std::string_view key, std::optional<std::string_view> value) const
    {
        try
        {
            auto &transformer = getTransformer(key);
            auto keyStr = sanitizeKey(key);
            return Setting(parseKey(keyStr), transformer.transform(value));
        }
        catch (std::exception &e)
        {
            throw SettingParserException{"Error for setting: '" + std::string{key} + "', value: '" +
                                         std::string{value ? *value : ""} + "' error: " + e.what()};
        }
    }

    INLINE IJsonTransformer &SettingParser::getTransformer(std::string_view &key) const
    {
        for (auto &[type, transformer] : _transformers)
        {
            if (tryExtractType(key, type))
            {
                return *transformer;
            }
        }
        return *_transformers.front().second;
    }

    INLINE bool SettingParser::tryExtractType(std::string_view &key, std::string_view type) const
    {
        if (details::utils::ignoreCaseEndsWith(key, type))
        {
            auto original = key;
            key.remove_suffix(type.size());
            if (tryExtractTypeMarker(key, _config.typeMarker))
            {
                return true;
            }
            if (tryExtractTypeMarker(key, _config.alternativeTypeMarker))
            {
                return true;
            }
            key = original;
        }
        return false;
    }

    INLINE bool SettingParser::tryExtractTypeMarker(std::string_view &key, std::string_view typeMarker) const
    {
        if (details::utils::endsWith(key, typeMarker))
        {
            key.remove_suffix(typeMarker.size());
            return true;
        }
        return false;
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
} // namespace sb::cf::details