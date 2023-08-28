#pragma once

#include <limits>
#include <cassert>

class ZBuffer {
public:
    ZBuffer(int width, int height) : width(width), height(height), pBuffer(std::make_unique<float[]>(width * height)) {
    }

    ~ZBuffer() = default;

    ZBuffer(const ZBuffer&) = delete;

    ZBuffer& operator=(const ZBuffer&) = delete;

    void Clear() {
        std::fill_n(pBuffer.get(), width * height, std::numeric_limits<float>::infinity());
    }

    float& At(int x, int y) {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);

        return pBuffer[y * width + x];
    }

    const float& At(int x, int y) const {
        return const_cast<ZBuffer*>(this)->At(x, y);
    }

    bool TestAndSet(int x, int y, float depth) {
        float& depthInBuffer = At(x, y);

        if (depth < depthInBuffer) {
            depthInBuffer = depth;

            return true;
        }

        return false;
    }

private:
    int width;
    int height;
    std::unique_ptr<float[]> pBuffer;
};
