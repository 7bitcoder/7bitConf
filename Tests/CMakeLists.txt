find_package(GTest REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(Tests
    ${SOURCES}
)

target_link_libraries(Tests PUBLIC
    GTest::gtest
    GTest::gmock
)

target_link_libraries(Tests PUBLIC
    7bitConf
)

file(GLOB_RECURSE FILES CONFIGURE_DEPENDS Files/*)
file(GLOB_RECURSE FILES_DIR CONFIGURE_DEPENDS Files/Directory/*)

file(COPY ${FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${FILES_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Directory)

gtest_discover_tests(Tests
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    PROPERTIES TIMEOUT 10)
