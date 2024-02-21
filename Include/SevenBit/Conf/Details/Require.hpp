#pragma once

#include <memory>
#include <string>
#include <type_traits>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    struct Require
    {
        template <class T> static void notNull(const T *ptr)
        {
            if (!ptr)
            {
                throw NullPointerException(std::string{"Pointer of type: '"} + typeid(T).name() + "' cannot be null");
            }
        }

        template <class T> static void notNull(const std::unique_ptr<T> &ptr) { notNull(ptr.get()); }

        template <class T> static void notNull(const std::shared_ptr<T> &ptr) { notNull(ptr.get()); }
    };
} // namespace sb::cf::details
