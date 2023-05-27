#pragma once

#include "SevenBit/Conf/JsonStreamConfiguration.hpp"

namespace sb::cf
{
    INLINE JsonStreamConfigurationProvider::JsonStreamConfigurationProvider(JsonStreamConfigurationSource source)
        : _source(std::move(source))
    {
    }

    INLINE void JsonStreamConfigurationProvider::load() { _configuration = getJsonFromFile(); }

    INLINE const JsonObject &JsonStreamConfigurationProvider::get() const { return _configuration; }

    INLINE JsonObject JsonStreamConfigurationProvider::getJsonFromFile()
    {
        auto json = tao::json::basic_from_stream<JsonTraits>(_source.getStream());
        if (!json.is_object())
        {
            throw BadStreamException("file does not contain json object");
        }
        return json.get_object();
    };

    INLINE JsonStreamConfigurationSource::JsonStreamConfigurationSource(std::istream &stream) : _stream(stream) {}

    INLINE std::istream &JsonStreamConfigurationSource::getStream() { return _stream; }

    INLINE IConfigurationProvider::Ptr JsonStreamConfigurationSource::build() const
    {
        return std::make_unique<JsonStreamConfigurationProvider>(*this);
    }
} // namespace sb::cf