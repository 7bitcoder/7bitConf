#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IObject.hpp"

namespace sb::cf
{
    template <class T> class ObjectHolder : public IObject
    {
        T _object;

        explicit ObjectHolder(const T &object) : _object(object) {}

        explicit ObjectHolder(T &&object) : _object(std::move(object)) {}

      public:
        using Ptr = std::unique_ptr<ObjectHolder>;

        [[nodiscard]] static Ptr from(const T &object) { return Ptr{new ObjectHolder{object}}; }

        [[nodiscard]] static Ptr from(T &&object) { return Ptr{new ObjectHolder{object}}; }

        [[nodiscard]] static ObjectHolder &castFrom(IObject &object) { return static_cast<ObjectHolder &>(object); }

        [[nodiscard]] static const ObjectHolder &castFrom(const IObject &object)
        {
            return static_cast<const ObjectHolder &>(object);
        }

        [[nodiscard]] static ObjectHolder &safeCastFrom(IObject &object)
        {
            return dynamic_cast<ObjectHolder &>(object);
        }

        [[nodiscard]] static const ObjectHolder &safeCastFrom(const IObject &object)
        {
            return dynamic_cast<const ObjectHolder &>(object);
        }

        [[nodiscard]] T &get() { return _object; }

        [[nodiscard]] const T &get() const { return _object; }
    };

} // namespace sb::cf
