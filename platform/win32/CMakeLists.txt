
add_library(win_platform
    window.cpp
    window_draw.cpp
)

target_link_libraries(win_platform
    d2d1
    dxgi
    dwrite
    app
)

target_include_directories(win_platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)