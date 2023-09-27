#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IObject.hpp"

namespace sb::cf
{
    template <class T> class ObjectHolder : public IObject
    {
      private:
        T _object;

        explicit ObjectHolder(const T &object) : _object(object) {}

        explicit ObjectHolder(T &&object) : _object(std::move(object)) {}

      public:
        using Ptr = std::unique_ptr<ObjectHolder<T>>;

        [[nodiscard]] static ObjectHolder<T>::Ptr from(const T &object)
        {
            return ObjectHolder<T>::Ptr{new ObjectHolder<T>{object}};
        }

        [[nodiscard]] static ObjectHolder<T>::Ptr from(T &&object)
        {
            return ObjectHolder<T>::Ptr{new ObjectHolder<T>{object}};
        }

        [[nodiscard]] static ObjectHolder<T> &castFrom(IObject &object)
        {
            return static_cast<ObjectHolder<T> &>(object);
        }

        [[nodiscard]] static const ObjectHolder<T> &castFrom(const IObject &object)
        {
            return static_cast<const ObjectHolder<T> &>(object);
        }

        [[nodiscard]] static ObjectHolder<T> &safeCastFrom(IObject &object)
        {
            return dynamic_cast<ObjectHolder<T> &>(object);
        }

        [[nodiscard]] static const ObjectHolder<T> &safeCastFrom(const IObject &object)
        {
            return dynamic_cast<const ObjectHolder<T> &>(object);
        }

        [[nodiscard]] T &get() { return _object; }

        [[nodiscard]] const T &get() const { return _object; }
    };

} // namespace sb::cf
