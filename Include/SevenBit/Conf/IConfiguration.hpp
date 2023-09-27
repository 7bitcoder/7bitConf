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

        [[nodiscard]] virtual const JsonValue &root() const = 0;

        [[nodiscard]] virtual const JsonObject &rootAsObject() const = 0;

        [[nodiscard]] virtual std::string toString(std::size_t indent = 1, std::string newLineMark = "\n") const = 0;

        [[nodiscard]] virtual const JsonValue *find(std::string_view key) const = 0;

        [[nodiscard]] virtual const JsonValue *deepFind(std::string_view key) const = 0;

        [[nodiscard]] virtual const JsonValue *deepFind(const std::vector<std::string_view> &key) const = 0;

        [[nodiscard]] virtual const JsonValue &operator[](std::string_view key) const { return deepAt(key); };

        [[nodiscard]] virtual const JsonValue &at(const std::string &key) const = 0;

        [[nodiscard]] virtual const JsonValue &deepAt(std::string_view key) const = 0;

        [[nodiscard]] virtual const JsonValue &deepAt(const std::vector<std::string_view> &key) const = 0;

        [[nodiscard]] virtual const JsonValue &operator[](const std::vector<std::string_view> &key) const
        {
            return deepAt(key);
        };

        [[nodiscard]] auto cbegin() const { return rootAsObject().cbegin(); }

        [[nodiscard]] auto cend() const { return rootAsObject().cend(); }

        [[nodiscard]] auto crbegin() const { return rootAsObject().crbegin(); }

        [[nodiscard]] auto crend() const { return rootAsObject().crend(); }

        [[nodiscard]] auto begin() const { return rootAsObject().begin(); }

        [[nodiscard]] auto end() const { return rootAsObject().end(); }

        [[nodiscard]] auto rbegin() const { return rootAsObject().rbegin(); }

        [[nodiscard]] auto rend() const { return rootAsObject().rend(); }

        virtual ~IConfiguration() = default;
    };

    inline std::ostream &operator<<(std::ostream &os, const IConfiguration &p) { return os << p.root(); }
} // namespace sb::cf