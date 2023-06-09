find_package(quom REQUIRED)

set(_7BIT_CONFIG_SINGLE_IN ${CMAKE_CURRENT_SOURCE_DIR}/SevenBitConfig.hpp.in)
set(_7BIT_CONFIG_SINGLE_OUT ${CMAKE_CURRENT_BINARY_DIR}/SevenBitConfig.hpp)

add_custom_command(OUTPUT ${_7BIT_CONFIG_SINGLE_OUT}
    COMMAND
    ${QUOM_EXECUTABLE} ${_7BIT_CONFIG_SINGLE_IN} ${_7BIT_CONFIG_SINGLE_OUT} -I ${_7BIT_CONFIG_HEADERS_DIR}
    DEPENDS
    ${_7BIT_CONFIG_ALL_HEADERS}
    COMMENT "Generating single header with Quom")

add_custom_target(GenerateSingleHeader
    ALL DEPENDS
    ${_7BIT_CONFIG_SINGLE_OUT}
    ${_7BIT_CONFIG_MAIN_HEADER}
)