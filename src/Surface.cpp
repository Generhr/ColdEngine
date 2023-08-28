#define FULL_WINTARD
#include "Surface.h"

#include "Win.h"

#include <gdiplus.h>
#include <sstream>
#include <vector>

#include <iostream>

#pragma comment(lib, "gdiplus.lib")

void Surface::PutPixelAlpha(unsigned int x, unsigned int y, const Color c) {
    assert(x < width);
    assert(y < height);
    // Load source pixel
    const Color d = GetPixel(x, y);

    // Blend channels
    const unsigned char rsltRed = (c.GetR() * c.GetA() + d.GetR() * (255u - c.GetA())) / 256u;
    const unsigned char rsltGreen = (c.GetG() * c.GetA() + d.GetG() * (255u - c.GetA())) / 256u;
    const unsigned char rsltBlue = (c.GetB() * c.GetA() + d.GetB() * (255u - c.GetA())) / 256u;

    // Pack channels back into pixel and fire pixel onto surface
    PutPixel(x, y, {rsltRed, rsltGreen, rsltBlue});
}

Surface Surface::FromFile(const std::wstring& name) {
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int pitch = 0;
    std::unique_ptr<Color[]> pBuffer = nullptr;
    {
        Gdiplus::Bitmap bitmap(name.c_str());

        if (bitmap.GetLastStatus() != Gdiplus::Status::Ok) {
            throw Exception(GET_EXCEPTION_FILE,
                GET_EXCEPTION_LINE,
                GET_EXCEPTION_COLUMN,
                L"Failed to load [" + name + L"].");
        }

        pitch = width = bitmap.GetWidth();
        height = bitmap.GetHeight();
        pBuffer = std::make_unique<Color[]>(width * height);

        for (unsigned int y = 0; y < height; y++) {
            for (unsigned int x = 0; x < width; x++) {
                Gdiplus::Color c;
                bitmap.GetPixel(x, y, &c);
                pBuffer[y * pitch + x] = Color(c.GetValue());
            }
        }
    }

    return Surface(width, height, pitch, std::move(pBuffer));
}

void Surface::Save(const std::wstring& filename) const {
    auto GetEncoderClsid = [&filename](const WCHAR* format, CLSID* pClsid) -> void {
        UINT num = 0;   // Number of image encoders
        UINT size = 0;  // Size of the image encoder array in bytes

        Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

        Gdiplus::GetImageEncodersSize(&num, &size);

        if (size == 0) {
            throw Exception(GET_EXCEPTION_FILE,
                GET_EXCEPTION_LINE,
                GET_EXCEPTION_COLUMN,
                L"Failed to get encoder for [" + filename + L"] (size == 0).");
        }

        pImageCodecInfo = static_cast<Gdiplus::ImageCodecInfo*>(malloc(size));

        if (pImageCodecInfo == nullptr) {
            throw Exception(GET_EXCEPTION_FILE,
                GET_EXCEPTION_LINE,
                GET_EXCEPTION_COLUMN,
                L"Failed to get encoder for [" + filename + L"] (failed to allocate memory).");
        }

        GetImageEncoders(num, size, pImageCodecInfo);

        for (UINT j = 0; j < num; ++j) {
            if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
                *pClsid = pImageCodecInfo[j].Clsid;
                free(pImageCodecInfo);
                return;
            }
        }

        free(pImageCodecInfo);

        throw Exception(GET_EXCEPTION_FILE,
            GET_EXCEPTION_LINE,
            GET_EXCEPTION_COLUMN,
            L"Failed to get encoder for [" + filename + L"] (failed to find matching encoder).");
    };

    CLSID bmpID;
    GetEncoderClsid(L"image/bmp", &bmpID);
    Gdiplus::Bitmap bitmap(width,
        height,
        pitch * sizeof(Color),
        PixelFormat32bppARGB,
        reinterpret_cast<BYTE*>(pBuffer.get()));

    if (bitmap.Save(filename.c_str(), &bmpID, nullptr) != Gdiplus::Status::Ok) {
        throw Exception(GET_EXCEPTION_FILE,
            GET_EXCEPTION_LINE,
            GET_EXCEPTION_COLUMN,
            L"Failed to save bitmap to [" + filename + L"].");
    }
}

void Surface::Copy(const Surface& src) {
    assert(width == src.width);
    assert(height == src.height);

    if (pitch == src.pitch) {
        memcpy(pBuffer.get(), src.pBuffer.get(), pitch * height * sizeof(Color));
    }
    else {
        for (unsigned int y = 0; y < height; y++) {
            memcpy(&pBuffer[pitch * y], &src.pBuffer[pitch * y], sizeof(Color) * width);
        }
    }
}
