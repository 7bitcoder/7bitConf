
include(GenerateExportHeader)

find_package(taocpp-json REQUIRED)

if(_7BIT_CONFIG_SHARED_LIB)
  add_library(7bitConfig SHARED
    Source.cpp ${_7BIT_CONFIG_ALL_HEADERS}
  )
elseif(_7BIT_CONFIG_STATIC_LIB)
  add_library(7bitConfig STATIC
    Source.cpp ${_7BIT_CONFIG_ALL_HEADERS}
  )
elseif(_7BIT_CONFIG_HEADER_ONLY_LIB)
  add_library(7bitConfig INTERFACE)
endif()

add_library(7bitConfig::7bitConfig ALIAS 7bitConfig)

if(_7BIT_CONFIG_HEADER_ONLY_LIB)
  target_link_libraries(7bitConfig INTERFACE
    taocpp::json
  )
else()
  target_link_libraries(7bitConfig
    taocpp::json
  )
endif()

target_include_directories(
  7bitConfig INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(7bitConfig PROPERTIES VERSION ${_7BIT_CONFIG_VERSION})
set_target_properties(7bitConfig PROPERTIES DEBUG_POSTFIX d)
