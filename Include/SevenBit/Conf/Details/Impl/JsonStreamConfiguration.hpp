#pragma once

#include <tao/json/from_stream.hpp>

#include "SevenBit/Conf/Details/Utils.hpp"
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

    INLINE IConfigurationProvider::Ptr JsonStreamConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<JsonStreamConfigurationProvider>(shared_from_this());
    }

    INLINE JsonStreamConfigurationProvider::JsonStreamConfigurationProvider(JsonStreamConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
        details::utils::assertPtr(_source);
    }

    INLINE void JsonStreamConfigurationProvider::load() { set(getJsonFromStream()); }

    INLINE JsonObject JsonStreamConfigurationProvider::getJsonFromStream()
    {
        auto json = tao::json::basic_from_stream<JsonTraits>(_source->getStream());
        if (!json.is_object())
        {
            throw BadStreamException("Stream does not contain json object");
        }
        return json.get_object();
    }
} // namespace sb::cf
