#pragma once

#include <string>
#include <optional>
#include <vector>
#include <functional>

#include "theme.hpp"
#include "grid_config.hpp"

// forward defining this because it exists within the config struct.
/**
 * A grid element for layout management.
 * Parameter is of type GridConfig:
 * 
 * std::string id = "";
 * std::vector<float> coordinates = std::vector<float> { 0.0f, 0.0f};
 * GridSize size{};
 * GridStyle style{};
 * Grid* container = nullptr;
 * 
 */
class Grid {
    public:
        /**
         * rubber ducking here:
         * I'll implement this similar to an html flex grid.
         * I need to be able to create rectangles, rounded rectangles, circles, and ovals.
         * I should be able to create nested grids.
         * each grid should be able to have a background color, and a border color and width.
         * To each grid I should be able to align the content, and add text.
         * I'll just add components sequentially, that way I can calculate the position of each component based on the previous components.
         * Each instance of grid will be a row, and each component rendered by that grid will be a column.
         * functions specific to OS, like rendering, will be implemented in the OS specific code, and called from the grid class to prevent reliance on windows or linux etc.
         */

        Grid(
            GridConfig config
        ) : 
            id(config.id),
            width(config.size.width),
            height(config.size.height),
            container(config.container),
            coordinates(config.coordinates),
            gridDirection(config.style.gridDirection),
            background_color(config.style.background_color),
            border_color(config.style.border_color),
            text(config.style.text),
            borderShape(config.style.borderShape),
            border_radius_width(config.style.border_radius_width),
            border_radius_height(config.style.border_radius_height)
        {
            if(container)
                container->addElement(this); // just prevents having to add explicitly
            else {
                // no container, set absolute coords to coords.
                this->absoluteCoordinates = config.coordinates;
                this->absoluteCoordinates.resize(4);
                this->absoluteCoordinates[2] = this->absoluteCoordinates[0] + this->getWidthPx();
                this->absoluteCoordinates[3] = this->absoluteCoordinates[1] + this->getHeightPx();
            }
        }

        // getters
        std::string getId() const { return id; };
        std::string& getText() {return text; };
        std::vector<float> getCoordinates() const { return coordinates; };
        std::vector<float>& getAbsoluteCoords() { return absoluteCoordinates; };
        std::vector<float> getAbsoluteCoords() const { return absoluteCoordinates; };
        std::vector<Grid*>& getElements() { return elements; };
        const std::vector<Grid*>& getElements() const { return elements; };
        std::optional<uint32_t> getBackgroundColor() const { return background_color; };
        std::optional<uint32_t> getBorderColor() const { return border_color; };
        std::optional<std::string> getText() const { return text; };
        std::optional<float> getBorderRadiusW() const { return border_radius_width; };
        std::optional<float> getBorderRadiusH() const { return border_radius_height; };
        BorderShape getBorderShape() const { return borderShape; };
        const GridDirection& getGridDirection() const { return gridDirection; };
        Grid* getContainer() const { return container; };
        float getWidthPx() const;
        float getHeightPx() const;
        bool& getInteractable() { return this->interactable; };

        // setters (some thought with std::move versus const ref)
        void setWidth(const std::string& width) { this->width = width; }
        void setHeight(const std::string& height) { this->height = height; }
        void setAbsoluteCoords (const std::vector<float>& coordinates) { this->absoluteCoordinates = coordinates; };
        void setInteractable(bool interactable) { this->interactable = interactable; };
        void setOnMouseOver(std::function<void()> onMouseOver) { this->onMouseOver = std::move(onMouseOver); };
        void setOnMouseOut(std::function<void()> onMouseOut) { this->onMouseOut = std::move(onMouseOut); };
        void setOnClick(std::function<void()> onClick) { this->onClick = std::move(onClick); };
        void setText(const std::string& text) { this->text = text; };

        // structural
        void addElement(Grid* elem);
        void addCol(Grid* col);
        void addRow(Grid* row);

        // helpers
        float strToWidthPx(std::string str, std::optional<float> containerWidth) const;
        float strToHeightPx(std::string str, std::optional<float> containerHeight) const;

        // mouse related
        
        std::function<void()> onMouseOver;
        std::function<void()> onMouseOut;
        std::function<void()> onClick;

        /// @brief Determines whether the grid element contains the provided coordinates. Returns true if contained.
        /// @param x 
        /// @param y 
        bool containsLocation(int x, int y);

        /// @brief Returns the leaf element at the provided coordinates. A
        ///         Assumptions made include: 
        ///         1. All elements of a grid are spacially contained within that Grid.
        ///         2. Grids do not overlap other grids.
        ///         3. We are only searching for interactable elements.
        /// @param x 
        /// @param y 
        Grid* getLeafAtLocation(int x, int y);

    private:
        // for referencing - efficient rendering.
        std::string id;

        // dimensions
        std::string width;
        std::string height;
        std::vector<float> coordinates;
        std::vector<float> absoluteCoordinates;

        // styling
        std::string text;
        std::optional<uint32_t> background_color;
        std::optional<uint32_t> border_color;
        float border_radius_width;
        float border_radius_height;
        BorderShape borderShape;
        GridDirection gridDirection;

        // mouse events.
        bool interactable = true;

        // structure
        Grid* container;
        std::vector<Grid*> elements = std::vector<Grid*>{};
};