#include <d2d1.h>
#include <d2d1helper.h> // apparently it's good standard practice to include imports even though they already exist in the header file.

#include "../window.hpp"
#include "window_impl.hpp"
#include "log.hpp"

void Window::rectangle(float left, float top, float right, float bottom, std::optional<uint32_t> background_color, std::optional<uint32_t> border_color) {

    uint32_t originalColour = getColourInt(); 

    D2D1_RECT_F rect = D2D1::RectF(left, top, right, bottom);
    
    if(background_color) {
        impl->pBrush->SetColor(D2D1::ColorF(*background_color));
        impl->pRenderTarget->FillRectangle(rect, impl->pBrush);
    } 

    if(border_color) { 
        impl->pBrush->SetColor(D2D1::ColorF(*border_color));
        impl->pRenderTarget->DrawRectangle(rect, impl->pBrush);
    }

    impl->pBrush->SetColor(D2D1::ColorF(originalColour)); // reset colour - preserves default colour.
}

void Window::roundedRectangle(float left, float top, float right, float bottom, float radiusX, float radiusY, std::optional<uint32_t> background_color, std::optional<uint32_t> border_color) {
        uint32_t originalColour = getColourInt(); 
    
        D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
            D2D1::RectF(left, top, right, bottom),
            radiusX,
            radiusY
        );
        
        if(background_color) {
            impl->pBrush->SetColor(D2D1::ColorF(*background_color));
            impl->pRenderTarget->FillRoundedRectangle(roundedRect, impl->pBrush);
        } 
    
        if(border_color) { 
            impl->pBrush->SetColor(D2D1::ColorF(*border_color));
            impl->pRenderTarget->DrawRoundedRectangle(roundedRect, impl->pBrush);
        }
    
        impl->pBrush->SetColor(D2D1::ColorF(originalColour)); // reset colour - preserves default colour.
}

void Window::ellipse(float centerX, float centerY, float radiusX, float radiusY, std::optional<uint32_t> background_color, std::optional<uint32_t> border_color) {
    uint32_t originalColour = getColourInt(); 

    D2D1_ELLIPSE ellipse = D2D1::Ellipse(
        D2D1::Point2F(centerX, centerY),
        radiusX,
        radiusY
    );

    if(background_color) {
        impl->pBrush->SetColor(D2D1::ColorF(*background_color));
        impl->pRenderTarget->FillEllipse(ellipse, impl->pBrush);
    }

    if(border_color) {
        impl->pBrush->SetColor(D2D1::ColorF(*border_color));
        impl->pRenderTarget->DrawEllipse(ellipse, impl->pBrush);
    }

    impl->pBrush->SetColor(D2D1::ColorF(originalColour)); // reset colour - preserves default colour.
}

uint32_t Window::getColourInt() {
    D2D1_COLOR_F dxColour = impl->pBrush->GetColor(); 
    return RGB(
        static_cast<uint8_t>(dxColour.r * 255),
        static_cast<uint8_t>(dxColour.g * 255),
        static_cast<uint8_t>(dxColour.b * 255)
    );
}

void Window::beginDraw() {
    impl->pRenderTarget->BeginDraw();
}

void Window::endDraw() {
    impl->pRenderTarget->EndDraw();
}

std::wstring utf8ToWide(const std::string& str)
{
    if (str.empty())
        return {};

    int size = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        nullptr,
        0
    );

    std::wstring result(size - 1, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        result.data(),
        size
    );

    return result;
}

// TODO: handle fonts, varaible text alignment. Container overflow.
void Window::text(float left, float top, float right, float bottom, std::string text) {
    auto wideText = utf8ToWide(text);

    impl->pRenderTarget->DrawText(
        wideText.c_str(),
        static_cast<UINT32>(wideText.size()),
        impl->pTextFormat,
        D2D1::RectF(
            left,
            top,
            right,
            bottom
        ),
        impl->pBrush
    );
}

void Window::loadImage(LPCWSTR fileName) {
    ID2D1Bitmap* bitmap = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;

    impl->wicFactory->CreateDecoderFromFilename(
        fileName,
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &decoder
    );

    decoder->GetFrame(0, &frame);

    impl->wicFactory->CreateFormatConverter(&converter);

    converter->Initialize(
        frame,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0,
        WICBitmapPaletteTypeCustom
    );

    // get frame + convert to 32bppPBGRA...

    impl->pRenderTarget->CreateBitmapFromWicBitmap(
        converter,
        nullptr,
        &bitmap
    );

    impl->pRenderTarget->DrawBitmap(bitmap);

    bitmap->Release();
    converter->Release();
    frame->Release();
    decoder->Release();
}

void Window::invalidateDraw(const RECT* rect) {
    InvalidateRect(impl->hwnd, rect, FALSE);
}