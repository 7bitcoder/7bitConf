#pragma once

#include <gmock/gmock.h>
#include <optional>
#include <string_view>

#include "SevenBit/Conf/IDeserializer.hpp"

struct DeserializerMock : public sb::cf::IDeserializer
{
    MOCK_METHOD((sb::cf::JsonValue), deserialize, (std::optional<std::string_view>), (const override));
};
