add_library(log
    log.cpp
)

target_include_directories(log PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
