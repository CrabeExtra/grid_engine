cmake_minimum_required(VERSION 3.10)

project(GridEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(platform)
add_subdirectory(layout)
add_subdirectory(log)
add_subdirectory(display)
add_subdirectory(input)

add_library(grid_engine INTERFACE)

target_link_libraries(grid_engine INTERFACE
    platform
    layout
    log 
    input
    display
)

install(DIRECTORY resources/
    DESTINATION resources
)