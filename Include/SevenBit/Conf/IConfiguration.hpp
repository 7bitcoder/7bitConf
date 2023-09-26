#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <tao/json/stream.hpp>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    struct IConfiguration
    {
        using Ptr = std::unique_ptr<IConfiguration>;

        virtual const JsonValue &root() const = 0;

        virtual const JsonObject &rootAsObject() const = 0;

        virtual std::string toString(std::size_t indent = 1, std::string newLineMark = "\n") const = 0;

        virtual const JsonValue *find(std::string_view key) const = 0;

        virtual const JsonValue *deepFind(std::string_view key) const = 0;

        virtual const JsonValue *deepFind(const std::vector<std::string_view> &key) const = 0;

        virtual const JsonValue &operator[](std::string_view key) const { return deepAt(key); };

        virtual const JsonValue &at(const std::string &key) const = 0;

        virtual const JsonValue &deepAt(std::string_view key) const = 0;

        virtual const JsonValue &deepAt(const std::vector<std::string_view> &key) const = 0;

        virtual const JsonValue &operator[](const std::vector<std::string_view> &key) const { return deepAt(key); };

        auto cbegin() const { return rootAsObject().cbegin(); }

        auto cend() const { return rootAsObject().cend(); }

        auto crbegin() const { return rootAsObject().crbegin(); }

        auto crend() const { return rootAsObject().crend(); }

        auto begin() const { return rootAsObject().begin(); }

        auto end() const { return rootAsObject().end(); }

        auto rbegin() const { return rootAsObject().rbegin(); }

        auto rend() const { return rootAsObject().rend(); }

        virtual ~IConfiguration() = default;
    };

    inline std::ostream &operator<<(std::ostream &os, const IConfiguration &p) { return os << p.root(); }
} // namespace sb::cf