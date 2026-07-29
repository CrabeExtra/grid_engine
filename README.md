# grid_engine

This is a grid engine used to streamline the GUI element of building DX windows-based apps. I will add a lot of extra features to this as time goes on.


## Usage

This has a set of includes. Ideally layout should not be exposed to the app level, neither should input (directly).

For non-install builds, please add the snippet below or some other method of obtaining grid_engine resources, and place the grid_engine folder within root/lib of your project.

file(GLOB LIB_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/lib/*)

foreach(LIB_DIR ${LIB_DIRS})
    if(IS_DIRECTORY ${LIB_DIR}/resources)
        message(STATUS "Found resources: ${LIB_DIR}/resources")
        get_filename_component(LIB_DIR_NAME ${LIB_DIR} NAME)
        add_custom_target(copy_${LIB_DIR_NAME}_resources ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIB_DIR}/resources/
            $<TARGET_FILE_DIR:CalculatorWin>/resources
        )
    endif()
endforeach()
## Notes

When creating a GUI grid. Please refer to layout/grid.hpp. Note that the parameter struct has defaults, the method for rendering, and note that for this to properly work, each grid element should be emplaced in the map under rootGrids in the display module.

Note that for Grids where you intend to set an interaction type function to anything non-null, you should also set the interactable field to true using setInteractable. This ensures the element is found when searching the mouseLocation.