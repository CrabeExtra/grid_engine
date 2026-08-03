#include "drawer_config.hpp"

static const GridConfig defaultButtonConfig = {
    .id = "drawer",
    .coordinates = { 0.0f, 0.0f },
    .size = {
        .width = "50px",
        .height = "50px"
    },
    .style = {
        .gridDirection = GridDirection::Col,
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
            
            auto& map = config.display.getRootGrids()[config.rootGridIndex];

            Grid* container = map[config.containerId];
            
            setContainer(container); // set the container of the button to the specified container grid.

            if(!config.leftHand) {
                setCoordinates({ container->getWidthPx() - getWidthPx(), 0.0f });
                setText("<");
            }

            std::vector<Grid*>& elements = container->getElements();
            elements.insert(elements.begin(), this); // add the button to the front of the elements vector so it is searched first for mouse input.
            map.emplace(getId(), this);
            
            setInteractable(true); // set the button to be interactable.

            // sidenav is off screen
            Grid* sideNav = new Grid({
                .id = "drawer_side_nav",
                .coordinates = { config.leftHand ? -200.0f : container->getWidthPx() + 2.0f, 0.0f },
                .size = {
                    .width = "199px",
                    .height = "100%"
                },
                .style = {
                    .gridDirection = GridDirection::Col,
                    .background_color = Theme::Surface,
                    .border_color = 0xFFFFFF,
                    .text = ""
                },
            });

            elements.insert(elements.begin(), sideNav);
            map.emplace(sideNav->getId(), sideNav);

            sideNav->setContainer(container);

            for(auto& menuElementConfig : config.menuElements) {
                menuElementConfig.container = sideNav; // set the container of the menu elements to the side nav grid.
                auto currentElem = new Grid(menuElementConfig); // create the menu elements and add them to the side nav grid.
                map.emplace(currentElem->getId(), currentElem);
            }
            
            setOnClick([this, sideNav, config]() {
                isOpen = !isOpen;

                auto& sideNavCoords = sideNav->getCoordinates();
                auto& buttonCoords = getCoordinates();

                // can certainly make this more elegant.
                if(isOpen) {
                    // open the drawer
                    sideNav->setCoordinates({ config.leftHand ? sideNavCoords[0] + 199.0f : sideNavCoords[0] - 199.0f, sideNavCoords[1] });
                    setCoordinates({ config.leftHand ? buttonCoords[0] + 199.0f : buttonCoords[0] - 199.0f, buttonCoords[1] });
                    setText(config.leftHand ? "<" : ">");
                } else {
                    // close the drawer
                    sideNav->setCoordinates({ config.leftHand ? sideNavCoords[0] - 199.0f : sideNavCoords[0] + 199.0f, sideNavCoords[1] });
                    setCoordinates({ config.leftHand ? buttonCoords[0] - 199.0f : buttonCoords[0] + 199.0f, buttonCoords[1] });
                    setText(config.leftHand ? ">" : "<");
                }

                config.display.invalidateWindow(); // trigger render.
            });
        }
    private:
        bool isOpen = false;
};