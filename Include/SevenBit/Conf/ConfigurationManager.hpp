#pragma once

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Configuration.hpp"
#include "SevenBit/Conf/ConfigurationBuilder.hpp"

namespace sb::cf
{
    class EXPORT ConfigurationManager : public ConfigurationBuilder, public Configuration
    {
      public:
        IConfigurationBuilder &add(IConfigurationSource::SPtr source) override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ConfigurationManager.hpp"
#endif
