#include <SevenBit/Conf.hpp>
#include <iostream>

using namespace sb::cf;

int main(const int argc, char **argv)
{
    const IConfiguration::Ptr configuration = ConfigurationBuilder{} //
                                                  .addAppSettings()
                                                  .addEnvironmentVariables()
                                                  .addCommandLine(argc, argv)
                                                  .build();

    const std::string value = configuration->at("MySetting").get_string();
    const std::string defaultLogLevel = configuration->deepAt("Logging:LogLevel:Default").get_string();
    const std::uint64_t secondArrayElement = configuration->deepAt("Array:1").get_unsigned();

    std::cout << "MySetting: " << value << std::endl;
    std::cout << "Default LogLevel: " << defaultLogLevel << std::endl;
    std::cout << "Second element in array: " << secondArrayElement << std::endl;

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;

    return 0;
}
