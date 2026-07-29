#pragma once

#include <string>
#include <optional>
#include <vector>
#include <functional>
#include <d2d1.h>
#include <d2d1helper.h>

#include "Windows.h"
#include "key.hpp"

/**
 * Generic Window class. This is kept OS and rendering API agnostic. Just contains interface required for the grid system. These functions
 * can be implemented per OS and used to create the same APP.
 */
class Window {
    public: 
        static constexpr int WINDOW_WIDTH = 360;
        static constexpr int WINDOW_HEIGHT = 480;
        static constexpr int X = CW_USEDEFAULT;
        static constexpr int Y = CW_USEDEFAULT;
        // core window initialisationa and loop. (might need to revisit these types when making OS agnostic - specifically when looking into Linux implementation)
        Window(
            HINSTANCE hInstance,
            int nCmdShow, 
            std::function<void()> render, 
            std::optional<std::function<void(float width, float height)>> onResize,
            std::optional<std::function<void(int x, int y)>> onMouseMove,
            std::optional<std::function<void(MouseButton button, int x, int y)>> onMouseDown,
            std::optional<std::function<void(MouseButton button, int x, int y)>> onMouseUp,
            std::optional<std::function<void(Key k)>> onKeyDown,
            std::optional<std::function<void(Key k)>> onKeyUp
        );
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
        void createWindow();
        void showWindow();
        void messageLoop();
        // window helpers
        int viewWidth();
        int viewHeight();
        // drawing functions
        uint32_t getColourInt();
        void rectangle(float left, float top, float right, float bottom, std::optional<uint32_t> background_color, std::optional<uint32_t> border_color);
        void roundedRectangle(float left, float top, float right, float bottom, float radiusX, float radiusY, std::optional<uint32_t> background_color, std::optional<uint32_t> border_color);
        void ellipse(float centerX, float centerY, float radiusX, float radiusY, std::optional<uint32_t> background_color, std::optional<uint32_t> border_color);
        void text(float left, float top, float right, float bottom, std::string text);
        void beginDraw();
        void endDraw();
        void loadImage(LPCWSTR fileName);
        void invalidateDraw(const RECT* rect = nullptr);
        void render();


    private:
        // This helps keep Windows/DirectX specific variables only visible to the win32 implementation
        // and any future Linux implementation only visible to the liux implementation. 
        struct Impl;
        Impl* impl;
        struct Image;
        std::vector<Image*> images;
        
};