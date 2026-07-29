add_library(layout
    grid.cpp
    grid_mouse.cpp
    grid_config.hpp
)


target_link_libraries(layout
    log
    platform
)

target_include_directories(layout PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)