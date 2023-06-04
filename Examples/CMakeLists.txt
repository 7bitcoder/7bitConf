
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

foreach(SOURCE ${SOURCES})
    get_filename_component(FILE_NAME ${SOURCE} NAME_WLE)

    add_executable(${FILE_NAME}
        ${SOURCE}
    )

    target_link_libraries(${FILE_NAME}
        7bitConfig
    )
endforeach()

file(GLOB_RECURSE FILES CONFIGURE_DEPENDS Files/*)

file(COPY ${FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})