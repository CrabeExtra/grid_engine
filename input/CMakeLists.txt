add_library(input
    input.cpp
)


target_link_libraries(input
    log
    platform
)

target_include_directories(input PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)