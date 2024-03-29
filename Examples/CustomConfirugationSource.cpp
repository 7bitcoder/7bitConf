#include <SevenBit/Conf.hpp>
#include <iostream>
#include <memory>

using namespace sb::cf;

class CustomConfigurationProvider : public IConfigurationProvider
{
    JsonObject _configuration;

  public:
    void load() override { _configuration = {{"mysettingOne", "value1"}, {"mysettingTwo", "value2"}}; }

    [[nodiscard]] const JsonObject &getConfiguration() const override { return _configuration; }
};

class CustomConfigurationSource : public IConfigurationSource
{
  public:
    IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override
    {
        return std::make_unique<CustomConfigurationProvider>();
    }
};

int main(int argc, char **argv)
{
    const IConfiguration::Ptr configuration =
        ConfigurationBuilder{}.add(std::make_unique<CustomConfigurationSource>()).build();

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;

    return 0;
}
