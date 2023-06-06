<div align="center">

  <img src="7bitConfig-logo.svg" alt="logo" width="500" height="auto" />
  <p>
    C++17 configuration provider library! 
  </p>

</div>
<br />

## About The Project

7BitConfig is a simple C++ configuration provider library, the main inspiration was the asp net core configuration mechanism.

### Built With

- [Taocpp Json](https://github.com/taocpp/json)
- [Google Test](https://github.com/google/googletest)

### Supported Platforms

7bitConfig requires client code and compiler compatible with the C++17 standard or newer.

The library is officially supported on the following platforms:

Operating systems:

- Linux
- macOS
- Windows

Compilers:

- gcc 7.0+
- clang 6.0+
- MSVC 2015+

### Installation

- Using Conan.io package manager - Recommended
  Download and install Conan* then install package*, see conan documentation for package installation guide
- Header only
  Download source code from the most recent release, copy the include folder into your project location, for example, copy into the '/SevenBitConfig' folder.
  Include this folder into the project, with CMake\_, u can use:

  ```cmake
  include_directories(/SevenBitConfig/Include)
  ```

- Header only - Single file
  Download SevenBitConfig.hpp header file from the most recent release,
  copy this file into your project location and include it.

- Building library as Static/Shared
  Download source code from the most recent release, build or install the project using CMake*,
  for more details see the `Building Library`* guide.

### Usage

This library provides centralized configuration management, and multiple configuration sources (files, environment variables, command line arguments, custom sources) are combined into one json object (see taocpp json documentation).

Create appsettings.json file in compiled executable directory:

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
}
```

The Example will print combined configuration from appsettings.json, environment variables and command line arguments. Source-adding order matters, the least source overrides the previous one.

#### Command Line

Command line configuration source is added using addCommandLine(argc, argv) builder method.

Argument pattern: [--]option[:nestedOption|arrayIndex...][!type]=value

Option prefix '--' is optional. Nested options are supported using the ':' separator. If the object is an array, numbers can be used to address the proper element, by default option value is saved as a string but other types are also supported using the '!' mark.

Example arguments:

- --MySetting="some value" - will override or create MySetting setting with "some value"
- --Logging:LogLevel:Default=Warning - will override or create in nested objects default log level setting with "Warning"
- --Strings:2=hello will override or create a third element in the Strings array setting with "hello"
- --Array:1!uint=123 will override the second element in Array with unsigned integer 123

##### Supported types:

- string - default type, could be specified explicitly
- uint - unsigned 64 bit integer
- int - 64 bit integer
- double
- bool - case insensitive "true" or "false" or number (non-zero is considered as true)
- json - json string for example {"hello": "value"}

#### Environment Variables

Environment variables configuration source is added using addEnvironmentVariables() builder method. This method will load all available variables. Call addEnvironmentVariables with a string to specify a prefix for environment variables:

addEnvironmentVariables("CUSTOM*PREFIX*")

it will load all environment variables with provided prefix (the prefix is removed in final configuration option names)

All rules for command line arguments are also valid for environment variables, some operating systems might not support '!' or ':' characters in variables in that case alternative separators can be used, alternative for ':' is '\_\_' (double underscore) and for '!' is '\_\_\_' (triple underscore)

option Array:2!uint=123 would be rewritten as Array\_\_2\_\_\_uint=123

#### Json File

Json file configuration source is added using addJsonFile(std::filesystem::path jsonFilePath) builder method. If the file does not exist method will throw an exception, call this method with an additional bool optional argument = true to prevent throwing an exception in this case

#### App Settings

App Settings configuration source is added using addAppSettings() builder method.
this source will use JSON file configuration source with "appsettings.json" file and isOptional = true parameters. addAppSettings is overloaded with an environment name string parameter

If the environment name is not empty this source will additionally load "appsettings.{environment name}.json", for example:

addAppSettings("myenv")

will additionally load "appsettings.myenv.json" after loading appsettings.json

#### Key Per File

Key Per File configuration source is added using addKeyPerFile(std::filesystem::path directoryPath) builder method.
this source will load all json files from the directory and save file contents under file name option name, the nested option is supported with \_\_ for example:

Assume directory:

MyDirectory/

- firstSetting.json
- second\_\_nested.json

addKeyPerFile("MyDirectory")

will load two json files, first under "firstSetting" option name, the second config will be stored in "nested" object which will be in "second" object

#### Json Stream

Json stream configuration source is added using addJsonStream(std::istream & stream) builder method. the stream must return the proper json file otherwise the method will throw an exception

#### Json Object

Json object configuration source is added using addJson(JsonObject json) builder method.

#### In Memory

In memory settings configuration source is added using addInMemory(std::vector<std::string, JsonValue>) builder method. keys is string with nested options separated with ':'

@7bitcoder Sylwester Dawida 2023
