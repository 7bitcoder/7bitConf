
set(gtest_build_tests OFF)
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(Tests
        ${SOURCES}
)

target_link_libraries(Tests PUBLIC
        GTest::gtest
        GTest::gmock
        7bitConf
)

file(GLOB_RECURSE FILES CONFIGURE_DEPENDS Files/*)
file(GLOB_RECURSE FILES_DIR CONFIGURE_DEPENDS Files/Directory/*)

file(COPY ${FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${FILES_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Directory)

file(COPY ${FILES} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${FILES_DIR} DESTINATION ${CMAKE_BINARY_DIR}/Directory)

gtest_discover_tests(Tests
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
