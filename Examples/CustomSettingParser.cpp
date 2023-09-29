#include <SevenBit/Conf.hpp>
#include <iostream>

using namespace sb::cf;

struct MyTypeDeserializer final : IDeserializer
{
    JsonValue deserialize(std::optional<std::string_view> value) const final { return value ? value : "emptyValue"; }
};

int main(int argc, char **argv)
{
    SettingParserConfig envParserConfig;
    envParserConfig.keySplitters.clear();
    envParserConfig.settingPrefixes.emplace_back("//");
    envParserConfig.defaultType = "myType";
    envParserConfig.throwOnUnknownType = false;

    ISettingParser::Ptr settingParser = SettingParserBuilder{} //
                                            .useConfig(std::move(envParserConfig))
                                            .useDefaultValueDeserializers()
                                            .useValueDeserializer("myType", std::make_unique<MyTypeDeserializer>())
                                            .build();

    IConfiguration::Ptr configuration = ConfigurationBuilder{} //
                                            .addAppSettings()
                                            .addEnvironmentVariables()
                                            .addCommandLine(argc, argv, std::move(settingParser))
                                            .build();

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;

    return 0;
}
