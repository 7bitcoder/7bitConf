#include <SevenBit/Config.hpp>
#include <iostream>

using namespace sb::cf;

int main(int argc, char **argv)
{
    IConfiguration::Ptr configuration = ConfigurationBuilder{}.addDefault(argc, argv).build();

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;
}