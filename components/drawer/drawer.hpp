#include "drawer_config.hpp"

static const GridConfig defaultButtonConfig = {
    .id = "drawer",
    .coordinates = { 0.0f, 0.0f },
    .size = {
        .width = "50px",
        .height = "50px"
    },
    .style = {
        .gridDirection = GridDirection::Row,
        .background_color = Theme::Surface,
        .border_color = 0xFFFFFF,
        .text = ">"
    },
    .container = nullptr,
};

/// @brief This is a component that renders a drawer on the specified container grid.
///         It becomes an interactive element of its container grid. 
class Drawer : public Grid {
    public:
        Drawer(DrawerConfig config) 
            : Grid(config.buttonConfig.id == "default" ? defaultButtonConfig : config.buttonConfig) {
            // add the button to the root grid.
            // create button first (in inherited Grid base constructor) so it isn't added to the end of the container's elements list.
            
            Grid* container = config.display.getRootGrids()[config.rootGridIndex][config.containerId];
            
            setContainer(container); // set the container of the button to the specified container grid.

            std::vector<Grid*>& elements = container->getElements();
            elements.insert(elements.begin(), this); // add the button to the front of the elements vector so it is searched first for mouse input.
            
            setInteractable(true); // set the button to be interactable.
        }
    private:
        
};