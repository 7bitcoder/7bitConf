#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    struct IObject
    {
        using Ptr = std::unique_ptr<IObject>;
        using SPtr = std::shared_ptr<IObject>;

        virtual ~IObject() = default;
    };

} // namespace sb::cf
