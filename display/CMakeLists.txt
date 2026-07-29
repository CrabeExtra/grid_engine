add_library(display
    display.cpp
)


target_link_libraries(display
    log
    platform
    layout
    input
)

target_include_directories(display PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)