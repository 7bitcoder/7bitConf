#pragma once

#include "SevenBit/Config/IConfigurationBuilder.hpp"
#include "SevenBit/Config/IConfigurationProvider.hpp"
#include "SevenBit/Config/IConfigurationSource.hpp"
#include "SevenBit/Config/JsonConfiguration.hpp"
#include "SevenBit/Config/ObjectHolder.hpp"

class CustomConfigSource : public sb::cf::IConfigurationSource
{
    sb::cf::IConfigurationProvider::Ptr build(sb::cf::IConfigurationBuilder &builder)
    {
        sb::cf::ObjectHolder<int>::safeCastFrom(*builder.getProperties()["counter"]).get()++;

        return sb::cf::JsonConfigurationSource::create({})->build(builder);
    }
};