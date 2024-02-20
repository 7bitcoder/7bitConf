add_executable(CliEcho Cli/echo.cpp)

target_link_libraries(CliEcho 7bitConf)

find_package(Python REQUIRED)

add_test(NAME EndToEnd.CommandLine
        COMMAND ${Python_EXECUTABLE} test.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        CONFIGURATIONS --exe $<TARGET_FILE:CliEcho>
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Cli)
