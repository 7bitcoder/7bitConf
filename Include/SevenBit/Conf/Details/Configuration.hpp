#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfiguration.hpp"
#include "SevenBit/Conf/IConfigurationProvider.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    class EXPORT Configuration : public IConfiguration
    {
      private:
        std::vector<IConfigurationProvider::Ptr> _providers;

        JsonValue _configuration = JsonObject{};

      public:
        using Ptr = std::unique_ptr<Configuration>;

        explicit Configuration(std::vector<IConfigurationProvider::Ptr> providers = {});

        Configuration(Configuration &&) = default;
        Configuration(const Configuration &) = delete;

        Configuration &operator=(Configuration &&) = delete;
        Configuration &operator=(const Configuration &) = delete;

        void load();

        void buildConfig(bool withLoad = false);

        [[nodiscard]] const std::vector<IConfigurationProvider::Ptr> &getProviders() const;

        [[nodiscard]] std::vector<IConfigurationProvider::Ptr> &getProviders();

        [[nodiscard]] std::string toString(std::size_t indent, std::string newLineMark) const override;

        [[nodiscard]] JsonValue &root();

        [[nodiscard]] const JsonValue &root() const override;

        [[nodiscard]] JsonObject &rootAsObject();

        [[nodiscard]] const JsonObject &rootAsObject() const override;

        [[nodiscard]] JsonValue &at(const std::string &key);

        [[nodiscard]] const JsonValue &at(const std::string &key) const override;

        [[nodiscard]] JsonValue *find(std::string_view key);

        [[nodiscard]] const JsonValue *find(std::string_view key) const override;

        [[nodiscard]] JsonValue *deepFind(std::string_view key);

        [[nodiscard]] const JsonValue *deepFind(std::string_view key) const override;

        [[nodiscard]] JsonValue *deepFind(const std::vector<std::string_view> &key);

        [[nodiscard]] const JsonValue *deepFind(const std::vector<std::string_view> &key) const override;

        [[nodiscard]] JsonValue &deepAt(std::string_view key);

        [[nodiscard]] const JsonValue &deepAt(std::string_view key) const override;

        [[nodiscard]] JsonValue &deepAt(const std::vector<std::string_view> &key);

        [[nodiscard]] const JsonValue &deepAt(const std::vector<std::string_view> &key) const override;

        [[nodiscard]] JsonValue &operator[](std::string_view key);

        [[nodiscard]] const JsonValue &operator[](std::string_view key) const override;

        [[nodiscard]] JsonValue &operator[](const std::vector<std::string_view> &key);

        [[nodiscard]] const JsonValue &operator[](const std::vector<std::string_view> &key) const override;

        [[nodiscard]] auto begin() { return rootAsObject().begin(); }

        [[nodiscard]] auto end() { return rootAsObject().end(); }

        [[nodiscard]] auto rbegin() { return rootAsObject().rbegin(); }

        [[nodiscard]] auto rend() { return rootAsObject().rend(); }

        [[nodiscard]] auto begin() const { return rootAsObject().begin(); }

        [[nodiscard]] auto end() const { return rootAsObject().end(); }

        [[nodiscard]] auto rbegin() const { return rootAsObject().rbegin(); }

        [[nodiscard]] auto rend() const { return rootAsObject().rend(); }

        [[nodiscard]] auto cbegin() const { return rootAsObject().cbegin(); }

        [[nodiscard]] auto cend() const { return rootAsObject().cend(); }

        [[nodiscard]] auto crbegin() const { return rootAsObject().crbegin(); }

        [[nodiscard]] auto crend() const { return rootAsObject().crend(); }

      private:
        [[nodiscard]] static JsonValue &throwNotFoundException(const std::vector<std::string_view> &key);

        [[nodiscard]] static JsonValue &throwNotFoundException(std::string_view key);
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/Configuration.hpp"
#endif
