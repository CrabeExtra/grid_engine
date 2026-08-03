add_library(drawer
    drawer.cpp
)

target_link_libraries(drawer
    layout
    display
)

target_include_directories(drawer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)