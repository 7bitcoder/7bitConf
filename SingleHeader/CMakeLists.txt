find_package(quom REQUIRED)

set(_7BIT_CONF_SINGLE_IN ${CMAKE_CURRENT_SOURCE_DIR}/SevenBitConf.hpp.in)
set(_7BIT_CONF_SINGLE_OUT ${CMAKE_CURRENT_BINARY_DIR}/SevenBitConf.hpp)

add_custom_command(OUTPUT ${_7BIT_CONF_SINGLE_OUT}
        COMMAND
        ${QUOM_EXECUTABLE} ${_7BIT_CONF_SINGLE_IN} ${_7BIT_CONF_SINGLE_OUT} -I ${_7BIT_CONF_HEADERS_DIR}
        DEPENDS
        ${_7BIT_CONF_ALL_HEADERS}
        COMMENT "Generating single header with Quom")

add_custom_target(GenerateSingleHeader
        ALL DEPENDS
        ${_7BIT_CONF_SINGLE_OUT}
        ${_7BIT_CONF_MAIN_HEADER}
)
