
add_subdirectory(drawer)

add_library(components INTERFACE)

target_link_libraries(components INTERFACE
    drawer
)