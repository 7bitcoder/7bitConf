#include <../../Include/SevenBit/Conf.hpp>
#include <iostream>

using namespace sb::cf;

int main(int argc, char **argv)
{
    IConfiguration::Ptr configuration = ConfigurationBuilder{} //
                                            .addCommandLine(argc, argv)
                                            .addEnvironmentVariables()
                                            .build();
    std::cout << *configuration;
    return 0;
}
