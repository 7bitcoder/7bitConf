add_executable(Echo echo.cpp)

target_link_libraries(Echo 7bitConf)

find_package(Python REQUIRED)

add_test(NAME E2E.Echo
        COMMAND ${Python_EXECUTABLE} echo_test.py $<TARGET_FILE:Echo>
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
