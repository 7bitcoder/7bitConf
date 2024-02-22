#include <SevenBit/Conf.hpp>
#include <iostream>

using namespace sb::cf;

int main(const int argc, char **argv)
{
    EnvironmentVarsParserConfig parserConfig;
    parserConfig.keySplitters.clear();
    parserConfig.typeMarkers.clear();

    const IConfiguration::Ptr configuration = ConfigurationBuilder{} //
                                                  .addAppSettings()
                                                  .addEnvironmentVariables("", std::move(parserConfig))
                                                  .addCommandLine(argc, argv)
                                                  .build();

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;

    return 0;
}
