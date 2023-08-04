#pragma once

#include "Colors.h"
#include "Win.h"
// #include "Rect.h"
#include "ColdException.h"

#include <assert.h>
#include <memory>
#include <string>

class Surface {
public:
    class Exception : public ColdException {
    public:
        using ColdException::ColdException;

        std::wstring GetFullMessage() const override {
            return GetNote() + L"\nAt: " + GetLocation();
        }

        std::wstring GetExceptionType() const override {
            return L"Surface Exception";
        }
    };

public:
    Surface(unsigned int width, unsigned int height, unsigned int pitch)
        : width(width), height(height), pitch(pitch), pBuffer(std::make_unique<Color[]>(pitch * height)) {
    }

    Surface(unsigned int width, unsigned int height) : Surface(width, height, width) {
    }

    Surface(Surface&& source) noexcept
        : width(source.width), height(source.height), pitch(source.pitch), pBuffer(std::move(source.pBuffer)) {
    }

    Surface(Surface&) = delete;

    ~Surface() = default;

    Surface& operator=(Surface&& donor) noexcept {
        width = donor.width;
        height = donor.height;
        pitch = donor.pitch;
        pBuffer = std::move(donor.pBuffer);
        donor.pBuffer = nullptr;
        return *this;
    }

    Surface& operator=(const Surface&) = delete;

    void Clear(Color fillValue) {
        memset(pBuffer.get(), fillValue.dword, pitch * height * sizeof(Color));
    }

    void Present(unsigned int dstPitch, BYTE* const pDst) const {
        for (unsigned int y = 0; y < height; y++) {
            memcpy(&pDst[dstPitch * y], &pBuffer[pitch * y], sizeof(Color) * width);
        }
    }

    void PutPixel(unsigned int x, unsigned int y, Color c) {
        assert(x < width);
        assert(y < height);
        pBuffer[y * pitch + x] = c;
    }

    void PutPixelAlpha(unsigned int x, unsigned int y, Color c);

    Color GetPixel(unsigned int x, unsigned int y) const {
        assert(x < width);
        assert(y < height);
        return pBuffer[y * pitch + x];
    }

    unsigned int GetWidth() const {
        return width;
    }

    unsigned int GetHeight() const {
        return height;
    }

    unsigned int GetPitch() const {
        return pitch;
    }

    Color* GetBufferPtr() {
        return pBuffer.get();
    }

    const Color* GetBufferPtrConst() const {
        return pBuffer.get();
    }

    static Surface FromFile(const std::wstring& name);
    void Save(const std::wstring& filename) const;
    void Copy(const Surface& src);

private:
    // calculate pixel pitch required for given byte alignment (must be multiple of 4 bytes)
    static unsigned int GetPitch(unsigned int width, unsigned int byteAlignment) {
        assert(byteAlignment % 4 == 0);
        const unsigned int pixelAlignment = byteAlignment / sizeof(Color);
        return width + (pixelAlignment - width % pixelAlignment) % pixelAlignment;
    }

    Surface(unsigned int width, unsigned int height, unsigned int pitch, std::unique_ptr<Color[]> pBufferParam)
        : width(width), height(height), pitch(pitch), pBuffer(std::move(pBufferParam)) {
    }

private:
    unsigned int width;
    unsigned int height;
    unsigned int pitch;  // pitch is in PIXELS, not bytes!
    std::unique_ptr<Color[]> pBuffer;
};
