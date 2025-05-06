cmake_minimum_required(VERSION 3.12)

project(
    club_monitor
    VERSION 1.0
    DESCRIPTION "C++ Year 2025 Club monitoring for Yadro intern"
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_subdirectory(bin)

# enable_testing()
# add_subdirectory(tests)
