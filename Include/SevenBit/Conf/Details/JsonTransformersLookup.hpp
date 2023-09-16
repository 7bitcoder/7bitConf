#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/Details/IJsonTransformer.hpp"

namespace sb::cf::details
{
    class JsonTransformersLookup
    {
      private:
        std::vector<std::pair<std::string_view, IJsonTransformer::Ptr>> _transformers;

      public:
        static JsonTransformersLookup createDefault()
        {
            JsonTransformersLookup lookup;
            lookup.add("string", std::make_unique<StringTransformer>());
            lookup.add("bool", std::make_unique<BoolTransformer>());
            lookup.add("int", std::make_unique<IntTransformer>());
            lookup.add("double", std::make_unique<DoubleTransformer>());
            lookup.add("json", std::make_unique<JsonTransformer>());
            lookup.add("uint", std::make_unique<UIntTransformer>());
            lookup.add("null", std::make_unique<NullTransformer>());
            return lookup;
        }

        void add(std::string_view type, IJsonTransformer::Ptr transformer)
        {
            _transformers.emplace_back(type, std::move(transformer));
        }

        const IJsonTransformer &getDefault() const { return *_transformers.front().second; }

        auto begin() { return _transformers.begin(); }

        auto end() { return _transformers.end(); }

        auto begin() const { return _transformers.begin(); }

        auto end() const { return _transformers.end(); }
    };
} // namespace sb::cf::details