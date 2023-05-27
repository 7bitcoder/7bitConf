#pragma once
#include <stdexcept>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Details/OptionsParser.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{

    INLINE OptionsParser::OptionsParser(OptionsParserCfg cfg) : _cfg(cfg) {}

    INLINE JsonObject OptionsParser::parseOption(std::string_view option) const
    {
        auto keySegments = utils::split(option, _cfg.optSplitter, 2);
        if (keySegments.size() == 1)
        {
            return parseOption(option, _cfg.defaultValue);
        }
        if (keySegments.size() == 2)
        {
            return parseOption(keySegments[0], keySegments[1]);
        }
        throw ConfigException("Wrong option format: " + std::string{option});
    }

    INLINE JsonObject OptionsParser::parseOption(std::string_view key, std::string_view value) const
    {
        return parseOption(key, parseValue(value));
    }

    INLINE JsonObject OptionsParser::parseOption(std::string_view key, JsonValue value) const
    {
        auto strKey = sanitizeKey(key);
        return parseOption(utils::split(strKey, _cfg.keySplitter), std::move(value));
    }

    INLINE JsonObject OptionsParser::parseOption(const std::vector<std::string_view> &key, JsonValue value) const
    {
        JsonObject result{};
        JsonObjectExt::insertInner(result, key, std::move(value));
        return result;
    }

    INLINE std::string OptionsParser::sanitizeKey(std::string_view key) const
    {
        if (utils::startsWith(key, _cfg.optPrefix))
        {
            key = key.substr(_cfg.optPrefix.size());
        }
        std::string str{key};
        utils::replaceAll(str, _cfg.alternativeKeySplitter, _cfg.keySplitter);
        return str;
    }

    INLINE JsonValue OptionsParser::parseValue(std::string_view &value) const
    {
        if (auto list = utils::split(value, _cfg.valueListSplitter); list.size() > 1)
        {
            JsonArray array;
            for (auto value : list)
            {
                array.push_back(std::string{value});
            }
            return array;
        }
        return std::string{value};
    }

} // namespace sb::cf