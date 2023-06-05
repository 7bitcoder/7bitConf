#pragma once

#include <memory>

#include "SevenBit/Config/LibraryConfig.hpp"

namespace sb::cf
{
    struct IObject
    {
        using Ptr = std::unique_ptr<IObject>;

        virtual ~IObject() = default;
    };

} // namespace sb::cf