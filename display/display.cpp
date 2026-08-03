#include "display.hpp"
#include "window.hpp"


void Display::drawGrid(Grid& g) {
    if(g.getAbsoluteCoords().size() < 1) return;
    
    // Draw grid itself
    std::vector<float> coords = g.getAbsoluteCoords();

    switch(g.getBorderShape()) {
        case BorderShape::Rectangle:
            window.rectangle(coords[0], coords[1], coords[2], coords[3], g.getBackgroundColor() ? g.getBackgroundColor() : std::nullopt, g.getBorderColor());
            break;
        case BorderShape::RoundedRectangle:
            window.roundedRectangle(coords[0], coords[1], coords[2], coords[3], g.getBorderRadiusW() ? *g.getBorderRadiusW() : 0.0f, g.getBorderRadiusH() ? *g.getBorderRadiusH() : 0.0f, g.getBackgroundColor(), g.getBorderColor());
            break;
        case BorderShape::Ellipse:
            window.ellipse(coords[0], coords[1], coords[2], coords[3], g.getBackgroundColor(), g.getBorderColor());
            break;
        case BorderShape::Circle:
            window.ellipse(coords[0], coords[1], coords[2], coords[3], g.getBackgroundColor(), g.getBorderColor());
            break;
    }

    auto text = g.getText();

    if(!text.empty()) {
        window.text(coords[0], coords[1], coords[2], coords[3], text);
    }
    
    // recurse and draw each nested grid.
    for (auto& nestedRow : g.getElements()) {
        drawGrid(*nestedRow);
    }
}

void Display::onResize(float width, float height) {
    setDrawId("root");
    invalidateWindow();
}

/// @brief On mouse move. On mouse move the currently hovered element will have its onMouseOver function called. While the previously hovered component (if no longer hovered) will have its onMouseOut function called.
/// @param x 
/// @param y 
void Display::onMouseMove(int x, int y) {
    
    Grid* cursorGrid = getMouseOver(x, y);

    if(!cursorGrid) return;

    std::string currentId = cursorGrid->getId();
    if(currentId != getHoverId()) {
        Grid* unHoveredGrid = rootGrids[activeGridIndex][getHoverId()];
        if(unHoveredGrid && unHoveredGrid->onMouseOut) {
            unHoveredGrid->onMouseOut();
        }
            
        if(cursorGrid && cursorGrid->onMouseOver) {
            cursorGrid->onMouseOver();
        }
            
        setHoverId(currentId);
    } 
}

Grid* Display::getMouseOver(int x, int y) {
    // just assuming these already exist. Note on floating "root" text here.
    std::unordered_map<std::string, Grid *>& map = rootGrids[activeGridIndex];
    Grid* g = map["root"]; // TODO: what to do with the floating "root".

    return g->getLeafAtLocation(x, y);
    
}   

// TODO: Handle specific mouseButton.

void Display::onLMouseInput(int x, int y) {
    if(input.mouseIsHeld(MouseButton::Left)) {
        onLMouseUp(x, y);
    } else {
        onLMouseDown(x, y);
    }
}

void Display::onLMouseDown(int x, int y) {
    Grid* cursorGrid = getMouseOver(x, y);

    if(!cursorGrid) return;

    setMouseDownId(cursorGrid->getId());
}

void Display::onLMouseUp(int x, int y) {
    Grid* cursorGrid = getMouseOver(x, y);

    if(!cursorGrid) return;

    if(getMouseDownId() == cursorGrid->getId()) {
        if(cursorGrid->onClick)
            cursorGrid->onClick();
    }
}

void Display::render() {

    // this will be called during the render loop, and will be responsible for drawing the current page layout to the screen. 
    window.beginDraw();
    
    try {
        if(rootGrids.empty()) {
            Log::warning(std::to_string(rootGrids.size()) + " root grids built.");
            Log::warning("No root grids to render.");
            throw std::exception("Rendering error. See logs.");
        }
        
        if(!getDrawId().empty()) {

            if(activeGridIndex >= rootGrids.size()) {
                Log::error("Attempting to access active grid out of bounds.");
                Log::error("Active grid: " + std::to_string(activeGridIndex));
                Log::error("Maximum active grid: " + rootGrids.size());
                throw std::exception("Rendering error. See logs.");
            }

            auto& map = rootGrids[activeGridIndex]; // if this errors it will catch anyway.
            auto mapEntry = map.find(getDrawId());

            if(mapEntry == map.end()) {
                Log::error("RootGrids improperly initialised. RootGrid entirely missing during rendering.");
                Log::error("Draw ID: " + getDrawId());
                throw std::exception("Rendering error. See logs.");
            }

            Grid* g = mapEntry->second;

            if(g == nullptr) {
                Log::error("RootGrid null during rendering.");
                Log::error("Draw ID: " + getDrawId());
                throw std::exception("Rendering error. See logs.");
            }
            
            drawGrid(*g); // draw the grid.

            //setDrawId(""); // Once drawn, set the draw ID to empty to safeguard unnecessary re-renders.
        }

        
    } catch(const std::exception& e) {
        std::string msg = std::string("Exception: ") + e.what();
        Log::error(msg);
    }
    
    window.endDraw();

}

void Display::initWindow() {
    window.createWindow();
    window.showWindow();
}

void Display::invalidateWindow() {
    window.invalidateDraw();
}

void Display::startMessageLoop() {
    window.messageLoop();
}

Grid* Display::addRootGrid(GridConfig config) {
    Grid* rootGrid = new Grid(config);
    rootGrids.back().emplace("root", rootGrid);
    return rootGrid;
}

Grid* _createGridAndAddToMap(GridConfig config, std::unordered_map<std::string, Grid*>& map, std::string containerId) {
    auto it = map.find(containerId);
    if (it != map.end()) {
        config.container = it->second; // set the container to the found grid.
        Grid* g = new Grid(config); // create the grid (adds element to container in constructor)
        map.emplace(config.id, g); // place on map for O(1) lookups if I need them.
        return g;
    }

    return nullptr;
}

Grid* Display::addGridElement(GridConfig config, const std::string& containerId, int rootGridIndex) {

    if (rootGridIndex >= -1 && rootGridIndex < static_cast<int>(rootGrids.size())) {
        if(rootGridIndex == -1) {
            for(auto& map : rootGrids) {
                Grid* g = _createGridAndAddToMap(config, map, containerId);
                if(g) return g;
            }
        } else {
            auto& map = rootGrids[rootGridIndex];
            Grid* g = _createGridAndAddToMap(config, map, containerId);
            if(g) return g;
        }

    } else {
        Log::error("Invalid rootGridIndex in addElement call, index: " + std::to_string(rootGridIndex) 
            + ", grid ID: " + config.id 
            + ", rootGrid size: " + std::to_string(rootGrids.size())
        );
    }

    Log::error("Element not added, no valid container found as specified: " + std::to_string(rootGridIndex) 
        + ", grid ID: " + config.id 
        + ", container ID: " + containerId
        + ", rootGrid size: " + std::to_string(rootGrids.size())
    );

    return nullptr;
}

