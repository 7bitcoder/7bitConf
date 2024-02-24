#include <SevenBit/Conf.hpp>
#include <iostream>

using namespace sb::cf;

int main(const int argc, char **argv)
{
    CommandLineParserConfig parserConfig;
    parserConfig.optionPrefixes = {"//"};
    parserConfig.typeMarkers.clear();

    const IConfiguration::Ptr configuration = ConfigurationBuilder{} //
                                                  .addAppSettings()
                                                  .addCommandLine(argc, argv, std::move(parserConfig))
                                                  .build();

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;

    return 0;
}
