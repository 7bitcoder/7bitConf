[![DevCI](https://github.com/7bitcoder/7bitConfig/actions/workflows/DevCI.yml/badge.svg?branch=dev)](https://github.com/7bitcoder/7bitConfig/actions/workflows/DevCI.yml)
[![MainCI](https://github.com/7bitcoder/7bitConfig/actions/workflows/MainCI.yml/badge.svg?branch=main)](https://github.com/7bitcoder/7bitConfig/actions/workflows/MainCI.yml)

<div align="center">
<img src="7bitConfig-logo.svg" alt="logo" width="500" height="auto" />
  <p>
    C++17 configuration provider library! 
  </p>

</div>
<br />

<details>
  <summary>
Table of Contents
  </summary>

- [About The Project](#about-the-project)
  - [Built With](#built-with)
  - [Supported Platforms](#supported-platforms)
- [Installation](#installation)
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
- [Build Library](#build-library)
  - [Build Library With Conan](#build-library-with-conan)
  - [Install Library](#install-library)
- [License](#license)
</details>

## About The Project

7BitConfig is a simple C++ centralized configuration provider library, the main inspiration was the asp net core configuration mechanism.

### Built With

- [Taocpp Json](https://github.com/taocpp/json)
- [Google Test](https://github.com/google/googletest)

### Supported Platforms

7bitConfig requires client code and a compiler compatible with the C++17 standard or newer.

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

All options except Conan, requires [taocpp json](https://github.com/taocpp/json) version 1.0.0-beta.13 to be already installed.

#### Using Conan Package Manager - Recommended

Download and install [Conan.io](https://conan.io/downloads.html) then install [package](https://conan.io/center/7bitconfig), see Conan documentation for the package installation guide

#### Header Only

Download source code from the most recent release and copy the include folder into your project location, for example, copy into the '/SevenBitConfig' folder. Include this folder into the project, with [CMake](https://cmake.org/), u can use:

```cmake
include_directories(SevenBitConfig/Include)
```

#### Header Only Single File

Download SevenBitConfig.hpp header file from the most recent release, copy this file into your project location and include it.

#### Building Library Locally

Download source code from the most recent release, build or install the project using [CMake](https://cmake.org/), for more details, see the [Building Library](#build-library) guide.

## Usage

This library provides centralized configuration management, and multiple configuration sources (files, environment variables, command line arguments, custom sources) are combined into one json object (see taocpp json [documentation](https://github.com/taocpp/json/tree/main/doc)).

Create the appsettings.json file in the compiled executable directory:

```json
{
  "Array": [1, 2, 3, 4, 5],
  "MySetting": "appsettings.json Value",
  "Logging": {
    "LogLevel": {
      "Default": "Information"
    }
  }
}
```

```cpp
#include <SevenBit/Config.hpp>
#include <iostream>

using namespace sb::cf;

int main(int argc, char **argv)
{
    IConfiguration::Ptr configuration = ConfigurationBuilder{}
                                            .addAppSettings()
                                            .addEnvironmentVariables()
                                            .addCommandLine(argc, argv)
                                            .build();

    std::string value = configuration->at("MySetting").get_string();
    std::string defaultLogLevel = configuration->deepAt("Logging:LogLevel:Default").get_string();
    std::uint64_t secondArrayElement = configuration->deepAt("Array:1").get_unsigned();

    std::cout << "MySetting: " << value << std::endl;
    std::cout << "Default LogLevel: " << defaultLogLevel << std::endl;
    std::cout << "Second element in array: " << secondArrayElement << std::endl;

    std::cout << "Configuration json:" << std::endl << std::setw(2) << *configuration;

    return 0;
}
```

The example will print combined configuration from appsettings.json, environment variables, and command-line arguments. Source-adding order matters, the least source overrides the previous one.

## Configuration Sources

The configuration builder class creates configuration from configuration sources, there are a few predefined sources ready to be used

### Command Line

The command line configuration source is added using the addCommandLine(argc, argv) builder method:

```cpp
auto configuration = ConfigurationBuilder{}.addCommandLine(argc, argv).build();
```

**Argument pattern:** [--]setting[:nestedSetting|arrayIndex...][!type]=[value]

Setting prefix '--' is optional. Nested settings are supported using the ':' separator. If the object is an array, numbers can be used to address the proper element. By default, setting values are saved as strings, but other types are also supported using the '!' mark. If a value is not provided, the default one will be used for the specified type, see [supported types](#supported-types)

Some arguments might be filtered using an overloaded method that accepts std::vector\<std::string_view\>
For example, we could pass to configuration only arguments that starts with "--SETTING":

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

#### Supported Types

type (default value) - description

- string ("") - default type, could be specified explicitly
- uint (0) - unsigned 64 bit integer
- int (0) - 64 bit integer
- double (0.0)
- bool (false) - case insensitive "true" or "false" or number (non-zero is considered as true)
- json (undefined) - json string for example {"hello": "value"}
- null (null) - null is used as a JSON value

#### Example Command Line Arguments

- --MySetting="hello" - will override or create a MySetting setting with the "hello" string value
- --Switch!bool=true - will override or create a Switch setting with a true bool value
- --Offset!double - will override or create Offset setting with the default 0.0 double value
- --Logging:LogLevel:Default=Warning - will override or create a nested setting with the string "Warning" value
- --Strings:2=hello - will override or create a third element in the Strings array setting with the string "hello" value
- --Array:1!uint=123 will override the second element in Array with the unsigned integer 123 value

### Environment Variables

The environment variables configuration source is added using the addEnvironmentVariables() builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addEnvironmentVariables().build();
```

This method will load all available environment variables. Call addEnvironmentVariables with a string to specify a prefix for environment variables:

```cpp
auto configuration = ConfigurationBuilder{}.addEnvironmentVariables("CUSTOM_PREFIX_").build();
```

It will load all environment variables with the provided prefix (the prefix is removed in the final configuration setting names)

All rules for command-line arguments are also valid for environment variables. Some operating systems might not support '!' or ':' characters in variables, in that case, alternative separators can be used. The alternative for ':' is '\_\_' (double underscore) and for '!' is '\_\_\_' (triple underscore)

Setting Array:2!uint=123 would be rewritten as Array\_\_2\_\_\_uint=123

### Json File

The JSON file configuration source is added using the addJsonFile(std::filesystem::path jsonFilePath) builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addJsonFile("configuration.json").build();
```

If the file does not exist, method will throw an exception, call this method with an additional bool optional argument = true to prevent throwing an exception in this case

```cpp
auto configuration = ConfigurationBuilder{}.addJsonFile("configuration.json", true).build();
```

### App Settings

The app settings configuration source is added using the addAppSettings() builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addAppSettings().build();
```

This source will use a JSON file configuration source with the "appsettings.json" file and isOptional = true parameter. addAppSettings is overloaded with an environment name string parameter. If the environment name is not empty, this source will additionally load "appsettings.{environment name}.json", for example:

```cpp
auto configuration = ConfigurationBuilder{}.addAppSettings("myenv").build();
```

It will additionally load "appsettings.myenv.json" after loading "appsettings.json"

### Key Per File

The key per file configuration source is added using the addKeyPerFile(std::filesystem::path directoryPath) builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addKeyPerFile("ConfigurationsDirectory").build();
```

This source will load all json files from the "ConfigurationsDirectory" directory and save file contents under the file name setting. The nested setting is supported with \_\_ for example:

Assume existing directory:

MyDirectory/

- firstSetting.json
- second\_\_nested.json

```cpp
auto configuration = ConfigurationBuilder{}.addKeyPerFile("MyDirectory").build();
```

It will load two json files, first under the "firstSetting" setting name, the second config will be stored in "nested" object which will be in the "second" object

Method will throw an exception if directory does not exist, call this method with an additional bool optional argument = true to prevent throwing an exception in this case

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

### Json Stream

A JSON stream configuration source is added using the addJsonStream(std::istream & stream) builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addJsonStream(stream).build();
```

The stream must return the proper JSON file otherwise, the method will throw an exception

### Json Object

A JSON object configuration source is added using the addJson(JsonObject json) builder method.

```cpp
auto configuration = ConfigurationBuilder{}.addJson({{"setting", "hello"}}).build();
```

### In Memory

In memory settings, the configuration source is added using addInMemory(std::vector<std::pair<std::string, JsonValue>>) builder method.
The first element in a pair can contain ':' to provide a nested setting

```cpp
auto configuration = ConfigurationBuilder{}.addInMemory({"setting:nested", "hello"}).build();
```

### Custom Configuration Source

A custom configuration source can be added using the add(IConfigurationSource::Sptr) builder method

A custom configuration source must implement IConfigurationSource

```cpp
#include <SevenBit/Config.hpp>
#include <iostream>

using namespace sb::cf;

class CustomConfigurationProvider : public IConfigurationProvider
{
  private:
    JsonObject _configuration;

  public:
    void load() override { _configuration = {{"mysettingOne", "value1"}, {"mysettingTwo", "value2"}}; }

    JsonObject &getConfiguration() override { return _configuration; }

    const JsonObject &getConfiguration() const override { return _configuration; }
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
    IConfiguration::Ptr configuration =
        ConfigurationBuilder{}.add(std::make_unique<CustomConfigurationSource>()).build();

    return 0;
}
```

## Build Library

The library can be built locally using [Cmake](https://cmake.org/), Library requires [Taocpp Json](https://github.com/taocpp/json) to be already installed, the best is to [Build Library With Conan](#build-library-with-conan)

Create a build directory and navigate to it:

```sh
mkdir build && cd build
```

Configure a CMake project

```sh
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

Using this command several cache variables can be set:

- \<cache variable name\>: [possible values] (default value) - Description
- \_7BIT_CONFIG_LIBRARY_TYPE: ["Shared", "Static", "HeaderOnly"] ("Static") - Library build type
- \_7BIT_CONFIG_BUILD_TESTS: ["ON", "OFF"] ("OFF") - Turn on to build tests (requires [Gtest](https://google.github.io/googletest/) to be installed, see [Build Library With Conan](#build-library-with-conan))
- \_7BIT_CONFIG_BUILD_EXAMPLES: ["ON", "OFF"] ("OFF") - Turn on to build examples
- \_7BIT_CONFIG_BUILD_SINGLE_HEADER: ["ON", "OFF"] ("OFF") - Turn on to build single header SevenBitConfig.hpp (requires Quom to be installed)
- \_7BIT_CONFIG_INSTALL: ["ON", "OFF"] ("OFF") - Turn on to install the library

To set cache variable pass additional option: -D\<cache variable name\>=[value],
for example, this command will set the library type to Static and will force examples built

```sh
cmake .. -DCMAKE_BUILD_TYPE=Release -D_7BIT_CONFIG_LIBRARY_TYPE=Static -D_7BIT_CONFIG_BUILD_EXAMPLES=true
```

Build the library using the command:

```sh
cmake --build .
```

### Build Library With Conan

Gtest library is added to the project using the Conan package manager ([Conan Installation](https://conan.io/downloads.html)),
If Conan was freshly installed, run the detect command:

```sh
conan profile detect
```

To install Conan packages, run this command in the library root folder:

```sh
conan install . --output-folder=build --build=missing
```

Navigate to the build directory:

```sh
cd build
```

Configure the CMake project and add the toolchain file as a CMAKE_TOOLCHAIN_FILE cache variable:

```sh
cmake .. -DCMAKE_TOOLCHAIN_FILE:PATH="./conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release -D_7BIT_CONFIG_BUILD_TESTS=ON
```

Build the library using the command:

```sh
cmake --build .
```

### Install Library

To install the library, set the additional cache variables \_7BIT_CONFIG_INSTALL=ON and specify the installation directory with CMAKE_INSTALL_PREFIX, then run the command

```sh
cmake --build . --config Release --target install
```

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

@7bitcoder Sylwester Dawida 2023
