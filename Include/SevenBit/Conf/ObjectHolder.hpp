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

        ObjectHolder(const T &object) : _object(object) {}

        ObjectHolder(T &&object) : _object(std::move(object)) {}

      public:
        using Ptr = std::unique_ptr<ObjectHolder<T>>;

        static ObjectHolder<T>::Ptr from(const T &object) { return ObjectHolder<T>::Ptr{new ObjectHolder<T>{object}}; }

        static ObjectHolder<T>::Ptr from(T &&object) { return ObjectHolder<T>::Ptr{new ObjectHolder<T>{object}}; }

        static ObjectHolder<T> &castFrom(IObject &object) { return static_cast<ObjectHolder<T> &>(object); }

        static const ObjectHolder<T> &castFrom(const IObject &object)
        {
            return static_cast<const ObjectHolder<T> &>(object);
        }

        static ObjectHolder<T> &safeCastFrom(IObject &object) { return dynamic_cast<ObjectHolder<T> &>(object); }

        static const ObjectHolder<T> &safeCastFrom(const IObject &object)
        {
            return dynamic_cast<const ObjectHolder<T> &>(object);
        }

        T &get() { return _object; }

        const T &get() const { return _object; }
    };

} // namespace sb::cf
