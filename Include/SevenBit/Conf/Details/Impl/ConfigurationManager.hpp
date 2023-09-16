#pragma once

#include "SevenBit/Conf/ConfigurationManager.hpp"
#include "SevenBit/Conf/Details/JsonExt.hpp"

namespace sb::cf
{
    INLINE IConfigurationBuilder &ConfigurationManager::add(IConfigurationSource::SPtr source)
    {
        ConfigurationBuilder::add(source);
        auto &provider = getProviders().emplace_back(source->build(*this));
        provider->load();
        details::JsonExt::deepMerge(rootAsObject(), std::move(provider->getConfiguration()));
        return *this;
    }
} // namespace sb::cf