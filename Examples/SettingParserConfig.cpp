#include <SevenBit/Conf.hpp>
#include <iostream>

using namespace sb::cf;

int main(int argc, char **argv)
{
    SettingParserConfig envParserConfig;
    envParserConfig.keySplitters.clear();
    envParserConfig.typeMarkers.clear();

    IConfiguration::Ptr configuration = ConfigurationBuilder{} //
                                            .addAppSettings()
                                            .addEnvironmentVariables("", std::move(envParserConfig))
                                            .addCommandLine(argc, argv)
                                            .build();

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;

    return 0;
}
