#pragma once

#include <gmock/gmock.h>

#include "SevenBit/Config/IConfigurationBuilder.hpp"

struct ConfigurationBuilderMock : public sb::cf::IConfigurationBuilder
{
    MOCK_METHOD((sb::cf::IConfigurationBuilder &), add, (sb::cf::IConfigurationSource::SPtr), (override));
    MOCK_METHOD((std::unordered_map<std::string, sb::cf::IObject::SPtr> &), getProperties, (), (override));
    MOCK_METHOD((const std::unordered_map<std::string, sb::cf::IObject::SPtr> &), getProperties, (), (const override));
    MOCK_METHOD((std::vector<sb::cf::IConfigurationSource::SPtr> &), getSources, (), (override));
    MOCK_METHOD((const std::vector<sb::cf::IConfigurationSource::SPtr> &), getSources, (), (const override));
    MOCK_METHOD((sb::cf::IConfiguration::Ptr), build, (), (override));
    MOCK_METHOD((void), clear, (), (override));
};