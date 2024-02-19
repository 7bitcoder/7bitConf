include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
set(gtest_build_tests OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(./Helpers)

if(_7BIT_CONF_BUILD_UNIT_TESTS)
        add_subdirectory(Unit)
endif()

if(_7BIT_CONF_BUILD_INTEGRATION_TESTS)
        add_subdirectory(Integration)
endif()

if(_7BIT_CONF_BUILD_E2E_TESTS)
        add_subdirectory(EndToEnd)
endif()

file(GLOB_RECURSE FILES CONFIGURE_DEPENDS ./Helpers/Files/*)
file(GLOB_RECURSE FILES_DIR CONFIGURE_DEPENDS ./Helpers/Files/Directory/*)

file(COPY ${FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${FILES_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Directory)

file(COPY ${FILES} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${FILES_DIR} DESTINATION ${CMAKE_BINARY_DIR}/Directory)