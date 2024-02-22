#pragma once

#include <gmock/gmock.h>

#include "SevenBit/Conf/IValueDeserializersMap.hpp"

struct ValueDeserializersMapMock : public sb::cf::IValueDeserializersMap
{
    MOCK_METHOD((const sb::cf::IDeserializer &), getDeserializerFor, (std::optional<std::string_view>),
                (const override));
};
