#pragma once

#include "Win.h"

#include "Colors.h"
// #include "Rect.h"
#include "EngineException.h"

#include <assert.h>
#include <memory>
#include <string>
#include <filesystem>

class Surface {
public:
    class Exception : public EngineException {
    public:
        using EngineException::EngineException;

        [[nodiscard]] std::wstring GetFullMessage() const override {
            return GetNote() + L"\nAt: " + GetLocation();
        }

        [[nodiscard]] std::wstring GetExceptionType() const override {
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

    [[nodiscard]] Color GetPixel(unsigned int x, unsigned int y) const {
        assert(x < width);
        assert(y < height);

        return pBuffer[y * pitch + x];
    }

    [[nodiscard]] unsigned int GetWidth() const {
        return width;
    }

    [[nodiscard]] unsigned int GetHeight() const {
        return height;
    }

    [[nodiscard]] unsigned int GetPitch() const {
        return pitch;
    }

    Color* GetBufferPtr() {
        return pBuffer.get();
    }

    [[nodiscard]] const Color* GetBufferPtrConst() const {
        return pBuffer.get();
    }

    static Surface FromFile(const std::wstring& name);
    static Surface FromMemory(const unsigned char* imageData, unsigned int width, unsigned int height);

    void Save(const std::wstring& filename) const;
    void Copy(const Surface& src);

private:
    // Calculate pixel pitch required for given byte alignment (must be multiple of 4 bytes)
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
    unsigned int pitch;  // Pitch is in PIXELS, not bytes!
    std::unique_ptr<Color[]> pBuffer;
};
