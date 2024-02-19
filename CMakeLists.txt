cmake_minimum_required(VERSION 3.15.0)

set(_7BIT_CONF_LIBRARY 7bitConf)

set(_7BIT_CONF_VERSION_MAJOR 1)
set(_7BIT_CONF_VERSION_MINOR 2)
set(_7BIT_CONF_VERSION_PATCH 1)

set(_7BIT_CONF_VERSION ${_7BIT_CONF_VERSION_MAJOR}.${_7BIT_CONF_VERSION_MINOR}.${_7BIT_CONF_VERSION_PATCH})

project(${_7BIT_CONF_LIBRARY} LANGUAGES CXX VERSION ${_7BIT_CONF_VERSION})

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Cmake")

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(Setup)
include(CMakeDependentOption)
include(GNUInstallDirs)

include_directories(${_7BIT_CONF_INCLUDE_DIR})

add_subdirectory(Source)

if(_7BIT_CONF_BUILD_UNIT_TESTS OR _7BIT_CONF_BUILD_INTEGRATION_TESTS OR _7BIT_CONF_BUILD_E2E_TESTS)
    enable_testing()

    add_subdirectory(Tests)
endif()

if(_7BIT_CONF_BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif()

if(_7BIT_CONF_BUILD_SINGLE_HEADER)
    add_subdirectory(SingleHeader)
endif()

if(_7BIT_CONF_INSTALL)
    set(PROJECT_CONFIG_IN ${CMAKE_CURRENT_SOURCE_DIR}/Cmake/7bitConfConfig.cmake.in)
    set(PROJECT_CONFIG_OUT ${CMAKE_CURRENT_BINARY_DIR}/7bitConfConfig.cmake)
    set(CONFIG_TARGETS_FILE 7bitConfConfigTargets.cmake)
    set(VERSIONS_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/7bitConfConfigVersion.cmake)
    set(EXPORT_DEST_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/7bitConf)

    install(DIRECTORY ${_7BIT_CONF_INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(
        TARGETS 7bitConf
        EXPORT 7bitConf
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    export(
        TARGETS 7bitConf
        NAMESPACE 7bitConf::
        FILE ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TARGETS_FILE})

    install(
        EXPORT 7bitConf
        EXPORT 7bitConf
        DESTINATION ${EXPORT_DEST_DIR}
        NAMESPACE 7bitConf::
        NAMESPACE 7bitConf::
        FILE ${CONFIG_TARGETS_FILE})

    include(CMakePackageConfigHelpers)

    configure_package_config_file(${PROJECT_CONFIG_IN} ${PROJECT_CONFIG_OUT}
        INSTALL_DESTINATION ${EXPORT_DEST_DIR})

    write_basic_package_version_file(${VERSIONS_CONFIG_FILE} COMPATIBILITY SameMajorVersion)

    install(FILES ${PROJECT_CONFIG_OUT} ${VERSIONS_CONFIG_FILE} DESTINATION ${EXPORT_DEST_DIR})

    export(PACKAGE 7bitConf)

    include(CPack)
endif()
