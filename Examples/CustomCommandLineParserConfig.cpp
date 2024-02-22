#include <SevenBit/Conf.hpp>
#include <iostream>

using namespace sb::cf;

struct MyTypeDeserializer final : IDeserializer
{
    [[nodiscard]] JsonValue deserialize(std::optional<std::string_view> value) const override
    {
        return value ? value : "emptyValue";
    }
};

int main(const int argc, char **argv)
{
    auto builderFunc = [](CommandLineParserBuilder &builder) {
        CommandLineParserConfig parserConfig;
        parserConfig.keySplitters.clear();
        parserConfig.optionPrefixes.emplace_back("//");
        parserConfig.defaultType = "myType";
        parserConfig.throwOnUnknownType = false;

        builder.useConfig(std::move(parserConfig))
            .useDefaultValueDeserializers()
            .useValueDeserializer("myType", std::make_unique<MyTypeDeserializer>());
    };

    const IConfiguration::Ptr configuration = ConfigurationBuilder{} //
                                                  .addAppSettings()
                                                  .addEnvironmentVariables()
                                                  .addCommandLine(argc, argv, builderFunc)
                                                  .build();

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;

    return 0;
}
