#pragma once

#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/JsonStreamConfiguration.hpp"

namespace sb::cf
{
    INLINE JsonStreamConfigurationSource::JsonStreamConfigurationSource(std::istream &stream) : _stream(stream) {}

    INLINE JsonStreamConfigurationSource::SPtr JsonStreamConfigurationSource::create(std::istream &stream)
    {
        return JsonStreamConfigurationSource::SPtr{new JsonStreamConfigurationSource{stream}};
    }

    INLINE std::istream &JsonStreamConfigurationSource::getStream() { return _stream; }

    INLINE IConfigurationProvider::Ptr JsonStreamConfigurationSource::build()
    {
        return std::make_unique<JsonStreamConfigurationProvider>(shared_from_this());
    }

    INLINE void JsonStreamConfigurationProvider::load() { set(getJsonFromStream()); }

    INLINE JsonObject JsonStreamConfigurationProvider::getJsonFromStream()
    {
        auto json = tao::json::basic_from_stream<JsonTraits>(_source->getStream());
        if (!json.is_object())
        {
            throw BadStreamException("file does not contain json object");
        }
        return json.get_object();
    };
} // namespace sb::cf