
include(GenerateExportHeader)

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
    add_library(7bitConf INTERFACE
            ../Include/SevenBit/Conf/IValueDeserializers.hpp)
endif ()

if (_7BIT_CONF_HEADER_ONLY_LIB)
    target_link_libraries(7bitConf INTERFACE
            taocpp::json
    )
else ()
    target_link_libraries(7bitConf
            taocpp::json
    )
endif ()

add_library(7bitConf::7bitConf ALIAS 7bitConf)

target_include_directories(
        7bitConf INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(7bitConf PROPERTIES VERSION ${_7BIT_CONF_VERSION})
set_target_properties(7bitConf PROPERTIES DEBUG_POSTFIX d)
