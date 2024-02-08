
include(GenerateExportHeader)

find_package(taocpp-json QUIET)
if(NOT taocpp-json_FOUND)
    include(FetchContent)

    FetchContent_Declare(
            taocpp-json
            GIT_REPOSITORY https://github.com/taocpp/json.git
            GIT_TAG 1.0.0-beta.14
            OVERRIDE_FIND_PACKAGE
    )
    FetchContent_MakeAvailable(taocpp-json)
endif()
find_package(taocpp-json REQUIRED)

if (_7BIT_CONF_SHARED_LIB)
    add_library(7bitConf SHARED
            Source.cpp ${_7BIT_CONF_ALL_HEADERS}
    )
elseif (_7BIT_CONF_STATIC_LIB)
    add_library(7bitConf STATIC
            Source.cpp ${_7BIT_CONF_ALL_HEADERS}
    )
elseif (_7BIT_CONF_HEADER_ONLY_LIB)
    add_library(7bitConf INTERFACE)
endif ()

if (_7BIT_CONF_HEADER_ONLY_LIB)
    target_link_libraries(7bitConf INTERFACE
            taocpp-json
    )
else ()
    target_link_libraries(7bitConf
            taocpp-json
    )
endif ()

add_library(7bitConf::7bitConf ALIAS 7bitConf)

set_target_properties(7bitConf PROPERTIES VERSION ${_7BIT_CONF_VERSION})
set_target_properties(7bitConf PROPERTIES DEBUG_POSTFIX d)

string(REPLACE ";" "$<SEMICOLON>" dirs "${_7BIT_CONF_INCLUDE_DIR}")
target_include_directories(7bitConf INTERFACE
        $<BUILD_INTERFACE:${dirs}>
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
