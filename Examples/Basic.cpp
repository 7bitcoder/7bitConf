#include <SevenBit/Config.hpp>
#include <iostream>

using namespace sb::cf;

int main(int argc, char **argv)
{
    IConfiguration::Ptr configuration = ConfigurationBuilder{} //
                                            .addAppSettings()
                                            .addEnvironmentVariables()
                                            .addCommandLine(argc, argv)
                                            .build();

    std::string value = configuration->at("MySetting").get_string();
    std::cout << "MySetting value: " << value << std::endl;

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;
}