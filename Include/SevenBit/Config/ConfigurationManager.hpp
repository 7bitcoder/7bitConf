#pragma once

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/Configuration.hpp"
#include "SevenBit/Config/ConfigurationBuilder.hpp"

namespace sb::cf
{
    class EXPORT ConfigurationManager : public ConfigurationBuilder, public Configuration
    {
      public:
        IConfigurationBuilder &add(IConfigurationSource::SPtr source) override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/ConfigurationManager.hpp"
#endif