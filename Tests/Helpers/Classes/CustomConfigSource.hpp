#pragma once

#include "SevenBit/Conf/IConfigurationBuilder.hpp"
#include "SevenBit/Conf/IConfigurationProvider.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/ObjectHolder.hpp"
#include "SevenBit/Conf/Sources/JsonConfiguration.hpp"


class CustomConfigSource : public sb::cf::IConfigurationSource
{
    sb::cf::IConfigurationProvider::Ptr build(sb::cf::IConfigurationBuilder &builder)
    {
        sb::cf::ObjectHolder<int>::safeCastFrom(*builder.getProperties()["counter"]).get()++;

        return sb::cf::JsonConfigurationSource::create({})->build(builder);
    }
};
