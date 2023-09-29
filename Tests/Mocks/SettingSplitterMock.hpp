#pragma once

#include <gmock/gmock.h>
#include <string_view>

#include "SevenBit/Conf/ISettingSplitter.hpp"

struct SettingSplitterMock : public sb::cf::ISettingSplitter
{
    MOCK_METHOD((sb::cf::ISettingSplitter::Result), split, (std::string_view), (const override));
};
