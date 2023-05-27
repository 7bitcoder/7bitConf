#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationRoot.hpp"

namespace sb::cf
{
    EXPORT class Configuration final : public IConfigurationRoot
    {
      private:
        std::vector<IConfigurationSource::SPtr> _sources;

        JsonObject _configuration;

      public:
        using Ptr = std::unique_ptr<Configuration>;

        Configuration(std::vector<IConfigurationSource::SPtr> sources);

        Configuration(Configuration &&) = default;
        Configuration(const Configuration &) = delete;

        Configuration &operator=(Configuration &&) = delete;
        Configuration &operator=(const Configuration &) = delete;

        const JsonObject &root() const;

        JsonObject &root();

        const JsonValue &at(const std::string &key) const;

        JsonValue &at(const std::string &key);

        const JsonValue *find(std::string_view key) const;

        JsonValue *find(std::string_view key);

        const JsonValue *findInner(std::string_view key) const;

        JsonValue *findInner(std::string_view key);

        const JsonValue *findInner(const std::vector<std::string_view> &key) const;

        JsonValue *findInner(const std::vector<std::string_view> &key);

        JsonValue &atInner(std::string_view key);

        const JsonValue &atInner(std::string_view key) const;

        JsonValue &atInner(const std::vector<std::string_view> &key);

        const JsonValue &atInner(const std::vector<std::string_view> &key) const;

        JsonValue &operator[](std::string_view key);

        const JsonValue &operator[](std::string_view key) const;

        JsonValue &operator[](const std::vector<std::string_view> &key);

        const JsonValue &operator[](const std::vector<std::string_view> &key) const;

        void reload();

        const std::vector<IConfigurationSource::SPtr> &getSources() const;

      private:
        JsonValue &throwNullPointnerException(const std::vector<std::string_view> &key) const;

        JsonValue &throwNullPointnerException(std::string_view key) const;
    };
} // namespace sb::cf