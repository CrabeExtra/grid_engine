
#include "Windows.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <functional>
#include <wincodec.h>
#include <dwrite.h>
#include <queue>

#include "../window.hpp"
#include "app.hpp"

struct MouseEvent {
    int x;
    int y;
};


struct Window::Impl {
    HWND hwnd = nullptr;
    HINSTANCE hInstance = nullptr;
    int nCmdShow;
    ID2D1HwndRenderTarget* pRenderTarget = nullptr;
    ID2D1SolidColorBrush* pBrush = nullptr;
    ID2D1Factory* factory = nullptr;
    IDWriteFactory* pDWriteFactory;
    IWICImagingFactory* wicFactory = nullptr;
    IDWriteTextFormat* pTextFormat = nullptr;
    std::function<void()> render = nullptr;
    std::function<void(float width, float height)> onResize;
    std::function<void(int x, int y)> onMouseMove;
    std::function<void(MouseButton button, int x, int y)> onMouseDown;
    std::function<void(MouseButton button, int x, int y)> onMouseUp;
    std::function<void(Key k)> onKeyDown;
    std::function<void(Key k)> onKeyUp;
    int height;
    int width;
    std::queue<MouseEvent> clickQueue;
};

struct Window::Image {
    
};