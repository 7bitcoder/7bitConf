[![DevCI](https://github.com/7bitcoder/7bitConf/actions/workflows/DevCI.yml/badge.svg?branch=dev)](https://github.com/7bitcoder/7bitConf/actions/workflows/DevCI.yml)
[![Windows](https://github.com/7bitcoder/7bitConf/actions/workflows/Windows.yml/badge.svg?branch=main)](https://github.com/7bitcoder/7bitConf/actions/workflows/Windows.yml)
[![Linux](https://github.com/7bitcoder/7bitConf/actions/workflows/Linux.yml/badge.svg?branch=main)](https://github.com/7bitcoder/7bitConf/actions/workflows/Linux.yml)
[![MacOs](https://github.com/7bitcoder/7bitConf/actions/workflows/MacOs.yml/badge.svg?branch=main)](https://github.com/7bitcoder/7bitConf/actions/workflows/MacOs.yml)
[![Conan Center](https://img.shields.io/conan/v/7bitconf)](https://conan.io/center/recipes/7bitconf)

<div align="center">
  <span>
    <img src="7bitConf-logo.svg" alt="logo" width="500" height="auto" />
    <p> C++17 centralized configuration provider library! </p>
  </span>
</div>

<br />

<details>
  <summary> Table of Contents </summary>

- [About The Project](#about-the-project)
    - [Built With](#built-with)
    - [Supported Platforms](#supported-platforms)
- [Installation](#installation)
    - [Using Cmake Fetch Content Api](#using-cmake-fetch-content-api---recommended)
    - [Using Conan Package Manager](#using-conan-package-manager)
    - [Header Only](#header-only)
    - [Header Only Single File](#header-only-single-file)
    - [Building Library Locally](#building-library-locally)
- [Usage](#usage)
- [Configuration Sources](#configuration-sources)
    - [Command Line](#command-line)
        - [Supported Types](#supported-types)
        - [Example Command Line Arguments](#example-command-line-arguments)
    - [Environment Variables](#environment-variables)
    - [Json File](#json-file)
    - [App Settings](#app-settings)
    - [Key Per File](#key-per-file)
    - [Json Stream](#json-stream)
    - [Json Object](#json-object)
    - [In Memory](#in-memory)
    - [Custom Configuration Source](#custom-configuration-source)
- [Command Line Parser Config](#command-line-parser-config)
    - [Cmd Config Usage Scenario](#cmd-config-usage-scenario)
- [Environment Variables Parser Config](#environment-variables-parser-config)
    - [Config Usage Scenario](#env-config-usage-scenario)
- [Custom Parsers](#custom-parsers)
    - [Advanced Usage Scenario](#advanced-usage-scenario)
- [Build Library](#build-library)
    - [Build Library With Conan](#build-library-with-conan)
    - [Install Library](#install-library)
- [License](#license)

</details>

## About The Project

7bitConf is a simple C++ centralized configuration provider library, the main inspiration was the ASP net core
configuration system.

### Built With

- [Taocpp Json](https://github.com/taocpp/json)
- [Google Test](https://github.com/google/googletest)

### Supported Platforms

7bitConf requires client code and a compiler compatible with the C++17 standard or newer.

The library is officially supported on the following platforms:

**Operating systems:**

- Linux
- macOS
- Windows

**Compilers:**

- gcc 8.0+
- clang 7.0+
- MSVC 2015+

## Installation

### Using Cmake Fetch Content Api - Recommended

Update CMakeLists.txt file with the following code

```cmake
include(FetchContent)
FetchContent_Declare(
        7bitConf
        GIT_REPOSITORY https://github.com/7bitcoder/7bitConf.git
        GIT_TAG v1.2.0
)
FetchContent_MakeAvailable(7bitConf)

target_link_libraries(Target 7bitConf::7bitConf)
```

### Using Conan Package Manager

Download and install [Conan.io](https://conan.io/downloads.html) then
install [package](https://conan.io/center/recipes/7bitconf), see Conan documentation for the package installation guide.

### Header Only

Download source code from the most recent release and copy the include folder into your project location, for example,
copy into the '/SevenBitConf' folder. Include this folder into the project, example with [CMake](https://cmake.org/):

```cmake
include_directories(SevenBitConf/Include)
```

### Header Only Single File

Download SevenBitConf.hpp header file from the most recent release, copy this file into the desired project location
and include it.

### Building Library Locally

Download source code from the most recent release, and build or install the project using [CMake](https://cmake.org/),
for more details, see the [Building Library](#build-library) guide.

## Usage

This library provides centralized configuration management, and multiple configuration sources (files, environment
variables, command line arguments, custom sources) are combined into one JSON object (see taocpp
json [documentation](https://github.com/taocpp/json/tree/main/doc)).

Create the appsettings.json file in the compiled executable directory:

```json
{
  "Array": [
    1,
    2,
    3
  ],
  "MySetting": "appsettings.json Value",
  "Logging": {
    "LogLevel": {
      "Default": "Information"
    }
  }
}
```

```cpp
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
```

The example will print combined configuration from appsettings.json, environment variables, and command-line arguments.
Source-adding order matters, the least source overrides the previous one.

## Configuration Sources

The configuration builder class creates configuration from configuration sources, there are a few predefined sources
ready to be used.

### Command Line

The command line configuration source is added using the addCommandLine(argc, argv) builder method:

```cpp
auto configuration = ConfigurationBuilder{}.addCommandLine(argc, argv).build();
```

**Argument patterns:**

- option[:nestedOption|arrayIndex...][!type]=[value]
- --option[:nestedOption|arrayIndex...][!type] [value]
- /option[:nestedOption|arrayIndex...][!type] [value]

Prefix ('--' or '/') is optional when the value is separated using '='. Nested settings are supported using the ':'
separator. If the object is an array, numbers can be used to address the proper element. By default, setting values are
saved as strings, but other types are also supported using the '!' mark. If a value is not provided, the default one
will be used for the specified type, see [supported types](#supported-types).

Some arguments might be filtered using an overloaded method that accepts std::vector\<std::string_view\>
This example shows how to pass only arguments that start with "--SETTING":

```cpp
std::vector<std::string_view> configArgs;
for (size_t i = 1; i < argc; ++i)
{
    std::string_view argView{argv[i]};
    if(argView.starts_with("--SETTING")) { // starts_with() c++ 20 feature
        configArgs.push_back(argView);
    }
}
auto configuration = ConfigurationBuilder{}.addCommandLine(configArgs).build();
```

The command line configuration source can be more customized with the additional addCommandLine method
arguments: [CommandLineParserConfig](#command-line-parser-config) or [SettingParser](#custom-parsers).

#### Supported Types

type (default value) - description

- string ("") - default type, could be specified explicitly.
- uint (0) - unsigned 64-bit integer.
- int (0) - 64-bit integer.
- double (0.0) - double.
- bool (false) - case-insensitive "true" or "false" or number (non-zero is considered as true).
- json (undefined) - JSON string for example {"hello": "value"}.
- null (null) - null is used as a JSON value.

#### Example Command Line Arguments

- MySetting - will override or create a MySetting setting with the default "" string value.
- MySetting="hello" - will override or create a MySetting setting with the "hello" string value.
- Switch!bool=true - will override or create a Switch setting with a true bool value.
- --MySetting hello - will override or create a MySetting setting with the "hello" string value.
- --Offset!double - will override or create an Offset setting with the default 0.0 double value.
- --Logging:LogLevel:Default Warning - will override or create a nested setting with the string "Warning" value.
- /Strings:2 hello - will override or create a third element in the Strings array setting with the string "hello"
  value.
- /Array:1!uint 123 will override the second element in Array with the unsigned integer 123 value.

### Environment Variables

The environment variables configuration source is added using the addEnvironmentVariables() builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addEnvironmentVariables().build();
```

This method will load all available environment variables, the better option is to load only prefixed ones (some
variables might be ill-formatted), call addEnvironmentVariables with a string to specify a prefix for environment
variables:

```cpp
auto configuration = ConfigurationBuilder{}.addEnvironmentVariables("CUSTOM_PREFIX_").build();
```

It will load all environment variables with the provided prefix (the prefix is removed in the final configuration
setting names).

All rules for command-line arguments are also valid for environment variables. Some operating systems might not
support '!' or ':' characters in variables, in that case, alternative separators can be used. The alternative for ':'
is '\_\_' (double underscore) and for '!' is '\_\_\_' (triple underscore).

Setting Array:2!uint=123 would be rewritten as Array\_\_2\_\_\_uint=123

Same as the command line source, the environment variables configuration source can be more customized with
additional addEnvironmentVariables method arguments: [EnvironmentVarsParserConfig](#environment-variables-parser-config)
or [SettingParser](#custom-parsers).

### Json File

The JSON file configuration source is added using the addJsonFile(std::filesystem::path jsonFilePath) builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addJsonFile("configuration.json").build();
```

If the file does not exist, the method will throw an exception, call this method with an additional bool optional
argument = true, to prevent throwing an exception in this case.

```cpp
auto configuration = ConfigurationBuilder{}.addJsonFile("configuration.json", true).build();
```

### App Settings

The app settings configuration source is added using the addAppSettings() builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addAppSettings().build();
```

This source will use a JSON file configuration source with the "appsettings.json" file and isOptional = true parameter.
addAppSettings is overloaded with an environment name string parameter. If the environment name is not empty, this
source will additionally load "appsettings.{environment name}.json", for example:

```cpp
auto configuration = ConfigurationBuilder{}.addAppSettings("myenv").build();
```

It will additionally load "appsettings.myenv.json" after loading "appsettings.json".

Various appsettings files can be prepared for different environments, environment name can be fetched from the system
environment variable using std::getenv(), for example:

```cpp
auto envName = std::getenv("MYAPP_ENVIRONMENT")
auto configuration = ConfigurationBuilder{}.addAppSettings(envName).build();
```

### Key Per File

The key per file configuration source is added using the addKeyPerFile(std::filesystem::path directoryPath) builder
method.

```cpp
auto configuration = ConfigurationBuilder{}.addKeyPerFile("ConfigurationsDirectory").build();
```

This source will load all JSON files from the "ConfigurationsDirectory" directory and save file contents under the file
name setting. The nested setting is supported with \_\_ for example:

Assume existing directory:

MyDirectory/

- firstSetting.json
- second\_\_nested.json

```cpp
auto configuration = ConfigurationBuilder{}.addKeyPerFile("MyDirectory").build();
```

It will load two JSON files, first under the "firstSetting" setting name, the second config will be stored in "nested"
object which will be in the "second" object.

The method will throw an exception if the directory does not exist, call this method with an additional bool optional
argument = true, to prevent throwing an exception in this case.

```cpp
auto configuration = ConfigurationBuilder{}.addKeyPerFile("MyDirectory", true).build();
```

Some files can be ignored with the string ignore prefix:

```cpp
auto configuration = ConfigurationBuilder{}.addKeyPerFile("MyDirectory", true, "ignoreFile_").build();
```

or with a functor:

```cpp
auto configuration = ConfigurationBuilder{}.addKeyPerFile("MyDirectory", true, [](const std::filesystem::path& file){ return file.filename().string().starts_with("ignoreFile_"); }).build();
```

### JSON Stream

A JSON stream configuration source is added using the addJsonStream(std::istream & stream) builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addJsonStream(stream).build();
```

The stream must return the proper JSON file otherwise, the method will throw an exception.

### JSON Object

A JSON object configuration source is added using the addJson(JsonObject json) builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addJson({{"setting", "hello"}}).build();
```

### In Memory

In memory settings, the configuration source is added using addInMemory(std::vector<std::pair<std::string, JsonValue>>)
builder method. The first element in a pair can contain ':' to provide a nested setting:

```cpp
auto configuration = ConfigurationBuilder{}.addInMemory({"setting:nested", "hello"}).build();
```

### Custom Configuration Source

A custom configuration source can be added using the add(IConfigurationSource::Sptr) builder method.

A custom configuration source must implement IConfigurationSource.

```cpp
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
```

## Command Line Parser Config

CommandLineParserConfig is a simple struct that contains the data used to configure the command line
parser, by default it is initialized with these values:

```cpp
struct CommandLineParserConfig
{
    std::vector<std::string_view> optionPrefixes = {"--", "/"};
    std::vector<std::string_view> optionSplitters = {"=", " "};
    std::vector<std::string_view> keySplitters = {":"};
    std::vector<std::string_view> typeMarkers = {"!"};
    std::string_view defaultType = "string";
    bool throwOnUnknownType = true;
    bool allowEmptyKeys = false;
};
```

This configuration allows specifying different behaviors of the command line parser.

Example option option:values:2!int=123 each part is marked as affected with ()

- optionPrefixes - list of possible option prefixes: (--)option:values:2!int=123
- optionSplitters - list of possible option splitters: --option:values:2!int(=)123
- keySplitters - list of possible key splitters: --option(:)values(:)2!int=123
- typeMarkers - list of possible type markers: --option:values:2(!)int=123
- defaultType - is the type that is used if the type was not specified explicitly in a variable: --option:values:2=123
- throwOnUnknownType - if the type was not recognized in the parsing phase then an exception will be thrown if in this
  case this setting is set to false default type will be used: --option:values:2!nonExistingType=123
- allowEmptyKeys - if set to true and empty keys are detected, an exception will be thrown: --option::2!int=123

### Cmd Config Usage Scenario

All options should be loaded without considering the type and with the custom option prefix '//', solution:

```cpp
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
```

## Environment Variables Parser Config

EnvironmentVarsParserConfig is a similar struct to the command line parser config with one difference there is no way to
set variable prefixes:

```cpp
struct EnvironmentVarsParserConfig
{
    std::vector<std::string_view> variableSplitters = {"="};
    std::vector<std::string_view> keySplitters = {":", "__"};
    std::vector<std::string_view> typeMarkers = {"!", "___"};
    std::string_view defaultType = "string";
    bool throwOnUnknownType = true;
    bool allowEmptyKeys = false;
};
```

This configuration allows specifying different behaviors of the environment variable parser.

Example environment variable option\_\_values\_\_2\_\_\_int=123 each part is marked as affected with ()

- variableSplitters - list of possible variable splitters: option\_\_values\_\_2\_\_\_int(=)123
- keySplitters - list of possible key splitters: option(\_\_)values(\_\_)2\_\_\_int=123
- typeMarkers - list of possible type markers: option\_\_values\_\_2(\_\_\_)int=123
- defaultType - is the type that is used if the type was not specified explicitly in a variable: optionvalues2=123
- throwOnUnknownType - if the type was not recognized in the parsing phase then an exception will be thrown if in this
  case this setting is set to false default type will be used: option\_\_values\_\_2\_\_\_nonExistingType=123
- allowEmptyKeys - if set to true and empty keys are detected, an exception will be thrown: option\_\_\_\_2\_\_\_int=123

Some system environment variables are prefixed with double underscore \_\_, in that case, the environment parser by
default will throw an exception because this prefix will be treated as a key splitter resulting in the empty first part
of keys, example \_\_SOME_SYSTEM_ENV=value will be parsed to "" and "SOME_SYSTEM_ENV" keys, in that case, set
allowEmptyKeys to true or disable keys splitting with clearing keySplitters config value, or filter out unwanted
variables with prefix passed to addEnvironmentVariables method

### Env Config Usage Scenario

All environment variables should be loaded as not nested objects (no key splitting) and without considering the type,
solution:

```cpp
#include <SevenBit/Conf.hpp>
#include <iostream>

using namespace sb::cf;

int main(int argc, char **argv)
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
```

## Custom Parsers

The library provides a CommandLineParserBuilder and EnvironmentVarsParserBuilder builder classes to create customized
parsers for command line and environment variables, builder allows using custom value deserializer, config,
settingSplitter, and valueDeserializersMap.

### Advanced Usage Scenario

All command line options should be loaded as not nested objects (no key splitting) with consideration of the new type "
myType" will set the option value to "emptyValue" string if no value was provided, myType should be used as the default
type, additional setting prefix should be considered '//' and default type should be also used if the type was not
recognized, solution:

```cpp
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
```

In this case, keySplitters is cleared to prevent extracting nested keys, the additional setting prefix is added '//',
and the default type is changed to custom type "myType", and with throwOnUnknownType set to false instead of throwing an
exception when type is not recognized, default will be used. CommandLineParserBuilder is being used to create a custom
command line parser with new config and custom valueDeserializer for type "myType", useDefaultValueDeserializers is used
to add predefined value deserializers (string, int, json ...).

## Build Library

The library can be built locally using [Cmake](https://cmake.org/), library
uses [Taocpp JSON](https://github.com/taocpp/json) if the library is not found it will be downloaded using Cmake fetch
api

Create a build directory and navigate to it:

```sh
mkdir build && cd build
```

Configure a CMake project:

```sh
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

Using this command, several cache variables can be set:

- \<cache variable name\>: [possible values] (default value) - Description
- \_7BIT_CONF_LIBRARY_TYPE: ["Shared", "Static", "HeaderOnly"] ("Static") - Library build type
- \_7BIT_CONF_BUILD_UNIT_TESTS: ["ON", "OFF"] ("OFF") - Turn on to build unit tests
- \_7BIT_CONF_BUILD_INTEGRATION_TESTS: ["ON", "OFF"] ("OFF") - Turn on to build integration tests
- \_7BIT_CONF_BUILD_E2E_TESTS: ["ON", "OFF"] ("OFF") - Turn on to build e2e tests
- \_7BIT_CONF_BUILD_ALL_TESTS: ["ON", "OFF"] ("OFF") - Turn on to build all tests (unit, integration and e2e)
- \_7BIT_CONF_BUILD_EXAMPLES: ["ON", "OFF"] ("OFF") - Turn on to build examples
- \_7BIT_CONF_BUILD_SINGLE_HEADER: ["ON", "OFF"] ("OFF") - Turn on to build single header SevenBitConf.hpp (requires
  Quom to be installed)
- \_7BIT_CONF_INSTALL: ["ON", "OFF"] ("OFF") - Turn on to install the library

To set the cache variable, pass the additional option: -D\<cache variable name\>=[value], for example, this command will
set the library type to Static and will force examples built

```sh
cmake .. -DCMAKE_BUILD_TYPE=Release -D_7BIT_CONF_LIBRARY_TYPE=Static -D_7BIT_CONF_BUILD_EXAMPLES=true
```

Build the library using the command:

```sh
cmake --build .
```

### Install Library

To install the library, set the additional cache variables \_7BIT_CONF_INSTALL=ON and specify the installation directory
with CMAKE_INSTALL_PREFIX, then run the command:

```sh
cmake --build . --config Release --target install
```

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

@7bitcoder Sylwester Dawida 2023
