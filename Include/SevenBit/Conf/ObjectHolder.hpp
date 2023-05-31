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

        T &getObject() { return _object; }

        const T &getObject() const { return _object; }
    };

} // namespace sb::cf
