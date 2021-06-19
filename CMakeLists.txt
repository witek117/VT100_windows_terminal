cmake_minimum_required(VERSION 3.15)

if(DEFINED ${ONLY_LIBRARY})
    add_subdirectory(src)
else()
    project(VT100 CXX C)
    set (CMAKE_CXX_STANDARD 17)

    add_subdirectory(external)
    add_subdirectory(src)

    add_executable(WindowsTerminal_unitTests
            tests/main.cpp
            tests/keyPress.cpp
            )

    target_link_libraries(WindowsTerminal_unitTests
            WindowsTerminal
            gtest
            gmock
            gtest_main
            )

    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=unused-parameter")

endif()
