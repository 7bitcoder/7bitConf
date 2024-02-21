#pragma once

#include <algorithm>

#include "SevenBit/Conf/Details/CommandLineParser.hpp"
#include "SevenBit/Conf/Details/Require.hpp"
#include "SevenBit/Conf/Details/StringUtils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

#include <SevenBit/Conf/Details/JsonExt.hpp>

namespace sb::cf::details
{
    INLINE CommandLineParser::CommandLineParser(ISettingSplitter::Ptr optionSplitter,
                                                IValueDeserializersMap::Ptr valueDeserializersMap,
                                                std::vector<std::string_view> optionPrefixes,
                                                const bool considerSeparated)
        : _optionSplitter(std::move(optionSplitter)), _valueDeserializersMap(std::move(valueDeserializersMap)),
          _optionPrefixes(std::move(optionPrefixes)), _considerSeparated(considerSeparated)
    {
        Require::notNull(_optionSplitter);
        Require::notNull(_valueDeserializersMap);
    }

    INLINE JsonObject CommandLineParser::parse(const std::vector<std::string_view> &arguments) const
    {
        JsonObject result;
        for (size_t index = 0; index < arguments.size(); ++index)
        {
            try
            {
                auto [keys, value] = parseArgument(arguments, index);
                JsonExt::updateWith(result, keys, std::move(value));
            }
            catch (const std::exception &e)
            {
                throw ConfigException("Parsing error for argument '" + std::string{arguments[index]} +
                                      "' error: " + e.what());
            }
        }
        return result;
    }

    INLINE CommandLineParser::ArgumentParseResult CommandLineParser::parseArgument(
        const std::vector<std::string_view> &arguments, size_t &index) const
    {
        auto argument = arguments[index];
        const auto hadOptionPrefix = tryRemoveOptionPrefix(argument);

        auto [keys, type, value] = _optionSplitter->split(argument);

        if (_considerSeparated && hadOptionPrefix && !value && index + 1 < arguments.size())
        {
            if (auto nextArgument = arguments[index + 1]; !tryGetOptionPrefix(nextArgument))
            {
                value = nextArgument;
                ++index;
            }
        }
        return {keys, _valueDeserializersMap->getDeserializerFor(type).deserialize(value)};
    }

    INLINE bool CommandLineParser::tryRemoveOptionPrefix(std::string_view &argument) const
    {
        if (const auto optionPrefix = tryGetOptionPrefix(argument))
        {
            argument.remove_prefix(optionPrefix->size());
            return true;
        }
        return false;
    }

    INLINE std::optional<std::string_view> CommandLineParser::tryGetOptionPrefix(std::string_view argument) const
    {
        for (auto &optionPrefix : _optionPrefixes)
        {
            if (StringUtils::startsWith(argument, optionPrefix))
            {
                return optionPrefix;
            }
        }
        return std::nullopt;
    }

} // namespace sb::cf::details
