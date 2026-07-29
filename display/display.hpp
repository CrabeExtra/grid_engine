#pragma once

#include "grid.hpp"
#include "input.hpp"
#include "window.hpp"
#include "key.hpp"

/// @brief This class is ideally what is exposed to the app rather than Window. It will contain
///         a list of functions required to draw a grid as well as everything required 
///         for inputs.
class Display
{
    public:
        Display(
            HINSTANCE hInstance, 
            int nCmdShow
        ) : 
            window(
                hInstance, 
                nCmdShow, 
                [this](){ this->render(); }, // TODO: remember to parse along 'this' from App: [this]() { this->render(); }, 
                [this](float width, float height) { this->onResize(width, height); } ,// as above: [this](float width, float height) { this->onResize(width, height); }
                [this](int x, int y) { this->onMouseMove(x, y); }, // 'this' is display from here.
                [this](MouseButton mouseButton, int x, int y) {
                    input.mouseDown(mouseButton, x, y);
                },
                [this](MouseButton mouseButton, int x, int y) {
                    input.mouseUp(mouseButton, x, y);
                },
                [this](Key key) {
                    input.keyDown(key);
                },
                [this](Key key) {
                    input.keyUp(key);
                }
            )
        {
            // think about this. Input already handles mouseDown and mouseUp. And there's only 
            input.setMouseButton(MouseButton::Left,
            [this](int x, int y) {
                onLMouseInput(x, y);
            });
        }

        void onResize(float width, float height);

        // draw

        void drawGrid(Grid& grid);

        std::string getDrawId() { return drawId; };
        
        void setDrawId(const std::string& drawId) { this->drawId = drawId; };

        void render();

        // Grid

        void setActiveGridIndex(int idx) { activeGridIndex = idx; };

        int getActiveGridIndex() { return activeGridIndex; };

        std::vector<std::unordered_map<std::string, Grid*>>& getRootGrids() { return rootGrids; } 

        // mouse 
        // TODO: could move the mousemove functions to inputand simply define behaviour here.
        void onMouseMove(int x, int y);
        Grid* getMouseOver(int x, int y);

        void onLMouseInput(int x, int y);
        void onLMouseDown(int x, int y);
        void onLMouseUp(int x, int y);
        
        void setHoverId(std::string id) { hoverId = id; };

        std::string getHoverId() { return hoverId; };

        void setMouseDownId(std::string id) { mouseDownId = id; };
        std::string getMouseDownId() { return mouseDownId; };

        // keyboard

        // Window

        void initWindow(); /// @brief Initialise the window before drawing.
        void invalidateWindow(); /// @brief Set window to redraw.
        void startMessageLoop(); /// @brief Start the window's message loop.
        void beginDraw() { window.beginDraw(); };
        void endDraw() { window.endDraw(); };
        void rectangle(float left, float top, float right, float bottom, std::optional<uint32_t> background_color, std::optional<uint32_t> border_color) { // TODO: make this more generic rather than a direct wrapper.
            window.rectangle(left, top, right, bottom, background_color, border_color); 
        }; 
        

    private: 
        Window window;
        Input input;
        std::vector<std::unordered_map<std::string, Grid*>> rootGrids; // root grids vector, each root grid is essentially a page layout. I've added unordered map
                                                                       // so that grids can be indexed by ID. Please note that the grid with ID "root" is always the root grid. The 'landing page'.
        
        std::string drawId = "root";
        int activeGridIndex = 0;
        
        std::string hoverId = "";
        std::string mouseDownId = "";  
};