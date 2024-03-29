#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ISettingSplitter.hpp"

namespace sb::cf::details
{

    class EXPORT SettingSplitter : public ISettingSplitter
    {
        const std::vector<std::string_view> _settingSplitters;
        const std::vector<std::string_view> _typeMarkers;
        const std::vector<std::string_view> _keySplitters;
        const bool _allowEmptyKeys;

      public:
        SettingSplitter(std::vector<std::string_view> settingSplitters, std::vector<std::string_view> typeMarkers,
                        std::vector<std::string_view> keySplitters, bool allowEmptyKeys = false);

        [[nodiscard]] Result split(std::string_view setting) const override;

        [[nodiscard]] const std::vector<std::string_view> &getSettingSplitters() const;

        [[nodiscard]] const std::vector<std::string_view> &getTypeMarkers() const;

        [[nodiscard]] const std::vector<std::string_view> &getKeySplitters() const;

        [[nodiscard]] bool getAllowEmptyKeys() const;

      private:
        [[nodiscard]] std::pair<std::string_view, std::optional<std::string_view>> splitSetting(
            std::string_view setting) const;

        [[nodiscard]] std::vector<std::string_view> splitKey(std::string_view key) const;

        [[nodiscard]] std::optional<std::string_view> tryExtractType(std::string_view &key) const;

        void checkKeys(const std::vector<std::string_view> &keys) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingSplitter.hpp"
#endif
