#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfiguration.hpp"
#include "SevenBit/Conf/IConfigurationProvider.hpp"

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

        std::string toString(std::size_t indent = 1, std::string newLineMark = "\n") const override;

        JsonValue &root();

        const JsonValue &root() const override;

        JsonObject &rootAsObject();

        const JsonObject &rootAsObject() const override;

        JsonValue &at(const std::string &key);

        const JsonValue &at(const std::string &key) const override;

        JsonValue *find(std::string_view key);

        const JsonValue *find(std::string_view key) const override;

        JsonValue *findInner(std::string_view key);

        const JsonValue *findInner(std::string_view key) const override;

        JsonValue *findInner(const std::vector<std::string_view> &key);

        const JsonValue *findInner(const std::vector<std::string_view> &key) const override;

        JsonValue &atInner(std::string_view key);

        const JsonValue &atInner(std::string_view key) const override;

        JsonValue &atInner(const std::vector<std::string_view> &key);

        const JsonValue &atInner(const std::vector<std::string_view> &key) const override;

        JsonValue &operator[](std::string_view key);

        const JsonValue &operator[](std::string_view key) const;

        JsonValue &operator[](const std::vector<std::string_view> &key);

        const JsonValue &operator[](const std::vector<std::string_view> &key) const;

        auto begin() { return rootAsObject().begin(); }

        auto end() { return rootAsObject().end(); }

        auto rBegin() { return rootAsObject().rbegin(); }

        auto rEnd() { return rootAsObject().rend(); }

      private:
        JsonValue &throwNullPointnerException(const std::vector<std::string_view> &key) const;

        JsonValue &throwNullPointnerException(std::string_view key) const;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/Configuration.hpp"
#endif