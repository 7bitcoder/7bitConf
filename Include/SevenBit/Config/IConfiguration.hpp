#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/Json.hpp"

namespace sb::cf
{
    struct IConfiguration
    {
        using Ptr = std::unique_ptr<IConfiguration>;

        virtual const JsonValue &root() const = 0;

        virtual const JsonObject &rootAsObject() const = 0;

        virtual std::string toString(std::size_t indent = 1, std::string newLineMark = "\n") const = 0;

        virtual const JsonValue *find(std::string_view key) const = 0;

        virtual const JsonValue *findInner(std::string_view key) const = 0;

        virtual const JsonValue *findInner(const std::vector<std::string_view> &key) const = 0;

        const JsonValue &operator[](std::string_view key) const { return atInner(key); };

        virtual const JsonValue &at(const std::string &key) const = 0;

        virtual const JsonValue &atInner(std::string_view key) const = 0;

        virtual const JsonValue &atInner(const std::vector<std::string_view> &key) const = 0;

        const JsonValue &operator[](const std::vector<std::string_view> &key) const { return atInner(key); };

        JsonObject::const_iterator cbegin() const { return rootAsObject().cbegin(); }

        JsonObject::const_iterator cEnd() const { return rootAsObject().cend(); }

        JsonObject::const_reverse_iterator crbegin() const { return rootAsObject().crbegin(); }

        JsonObject::const_reverse_iterator crend() const { return rootAsObject().crend(); }

        JsonObject::const_iterator begin() const { return rootAsObject().begin(); }

        JsonObject::const_iterator end() const { return rootAsObject().end(); }

        JsonObject::const_reverse_iterator rbegin() const { return rootAsObject().rbegin(); }

        JsonObject::const_reverse_iterator rend() const { return rootAsObject().rend(); }

        virtual ~IConfiguration() = default;
    };

    inline std::ostream &operator<<(std::ostream &os, const IConfiguration &p) { return os << p.root(); }
} // namespace sb::cf