#include <SevenBit/Config.hpp>
#include <iostream>

using namespace sb::cf;

int main(int argc, char **argv)
{
    IConfiguration::Ptr configuration = ConfigurationBuilder{}.addDefault(argc, argv).build();

    std::string value = configuration->at("MySetting").get_string();

    std::cout << "MySetting value: " << value;
}