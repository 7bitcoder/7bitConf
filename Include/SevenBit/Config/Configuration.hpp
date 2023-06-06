#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Config/Json.hpp"
#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/IConfiguration.hpp"
#include "SevenBit/Config/IConfigurationProvider.hpp"

namespace sb::cf
{
    EXPORT class Configuration : public IConfiguration
    {
      private:
        std::vector<IConfigurationProvider::Ptr> _providers;

        JsonValue _configuration = JsonObject{};

      public:
        using Ptr = std::unique_ptr<Configuration>;

        Configuration(std::vector<IConfigurationProvider::Ptr> providers = {});

        Configuration(Configuration &&) = default;
        Configuration(const Configuration &) = delete;

        Configuration &operator=(Configuration &&) = delete;
        Configuration &operator=(const Configuration &) = delete;

        void reload();

        const std::vector<IConfigurationProvider::Ptr> &getProviders() const;

        std::vector<IConfigurationProvider::Ptr> &getProviders();

        std::string toString(std::size_t indent = 1, std::string newLineMark = "\n") const override;

        JsonValue &root();

        const JsonValue &root() const override;

        JsonObject &rootAsObject();

        const JsonObject &rootAsObject() const override;

        JsonValue &at(const std::string &key);

        const JsonValue &at(const std::string &key) const override;

        JsonValue *find(std::string_view key);

        const JsonValue *find(std::string_view key) const override;

        JsonValue *deepFind(std::string_view key);

        const JsonValue *deepFind(std::string_view key) const override;

        JsonValue *deepFind(const std::vector<std::string_view> &key);

        const JsonValue *deepFind(const std::vector<std::string_view> &key) const override;

        JsonValue &deepAt(std::string_view key);

        const JsonValue &deepAt(std::string_view key) const override;

        JsonValue &deepAt(const std::vector<std::string_view> &key);

        const JsonValue &deepAt(const std::vector<std::string_view> &key) const override;

        JsonValue &operator[](std::string_view key);

        const JsonValue &operator[](std::string_view key) const;

        JsonValue &operator[](const std::vector<std::string_view> &key);

        const JsonValue &operator[](const std::vector<std::string_view> &key) const;

        auto begin() { return rootAsObject().begin(); }

        auto end() { return rootAsObject().end(); }

        auto rbBegin() { return rootAsObject().rbegin(); }

        auto rend() { return rootAsObject().rend(); }

      private:
        JsonValue &throwNullPointnerException(const std::vector<std::string_view> &key) const;

        JsonValue &throwNullPointnerException(std::string_view key) const;
    };
} // namespace sb::cf

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/Configuration.hpp"
#endif