#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    struct IObject
    {
        using Ptr = std::unique_ptr<IObject>;

        ~IObject() = default;
    };

} // namespace sb::cf
