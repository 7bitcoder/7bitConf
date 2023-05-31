#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    struct IConfiguration
    {
        using Ptr = std::unique_ptr<IConfiguration>;

        virtual const JsonObject &root() const = 0;

        virtual const JsonValue *find(std::string_view key) const = 0;

        virtual const JsonValue *findInner(std::string_view key) const = 0;

        virtual const JsonValue *findInner(const std::vector<std::string_view> &key) const = 0;

        const JsonValue &operator[](std::string_view key) const { return atInner(key); };

        virtual const JsonValue &at(const std::string &key) const = 0;

        virtual const JsonValue &atInner(std::string_view key) const = 0;

        virtual const JsonValue &atInner(const std::vector<std::string_view> &key) const = 0;

        const JsonValue &operator[](const std::vector<std::string_view> &key) const { return atInner(key); };

        JsonObject::const_iterator cBegin() const { return root().cbegin(); }

        JsonObject::const_iterator cEnd() const { return root().cend(); }

        JsonObject::const_reverse_iterator crBegin() const { return root().crbegin(); }

        JsonObject::const_reverse_iterator crEnd() const { return root().crend(); }

        JsonObject::const_iterator begin() const { return root().begin(); }

        JsonObject::const_iterator end() const { return root().end(); }

        JsonObject::const_reverse_iterator rBegin() const { return root().rbegin(); }

        JsonObject::const_reverse_iterator rEnd() const { return root().rend(); }

        virtual ~IConfiguration() = default;
    };
} // namespace sb::cf