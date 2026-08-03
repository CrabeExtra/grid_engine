#include "grid.hpp"
#include "display.hpp"

struct DrawerConfig {
    std::vector<GridConfig> menuElements;
    Display& display;
    GridConfig buttonConfig = {
        .id = "default"
    };
    bool leftHand = true;
    int rootGridIndex = 0;
    std::string containerId = "root";
};