#pragma once

#include "LinearAlgebra/Vector3.h"

class Color {
public:
    constexpr Color() : dword() {
    }

    constexpr Color(const Color& col) = default;

    explicit constexpr Color(unsigned int dw) : dword(dw) {
    }

    constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
        : dword((x << 24u) | (r << 16u) | (g << 8u) | b) {
    }

    constexpr Color(unsigned char r, unsigned char g, unsigned char b) : dword((r << 16u) | (g << 8u) | b) {
    }

    constexpr Color(Color col, unsigned char x) : Color((x << 24u) | col.dword) {
    }

    Color(Color&&) = default;

    explicit Color(const Vec3& cf)
        : Color(static_cast<unsigned char>(cf[0]),
              static_cast<unsigned char>(cf[1]),
              static_cast<unsigned char>(cf[2])) {
    }

    ~Color() = default;

    Color& operator=(Color&& color) = default;
    Color& operator=(const Color& color) = default;

    explicit operator Vec3() const {
        return {static_cast<float>(GetR()), static_cast<float>(GetG()), static_cast<float>(GetB())};
    }

    [[nodiscard]] constexpr unsigned char GetX() const {
        return dword >> 24u;
    }

    [[nodiscard]] constexpr unsigned char GetA() const {
        return GetX();
    }

    [[nodiscard]] constexpr unsigned char GetR() const {
        return (dword >> 16u) & 0xFFu;
    }

    [[nodiscard]] constexpr unsigned char GetG() const {
        return (dword >> 8u) & 0xFFu;
    }

    [[nodiscard]] constexpr unsigned char GetB() const {
        return dword & 0xFFu;
    }

    void SetX(unsigned char x) {
        dword = (dword & 0xFFFFFFu) | (x << 24u);
    }

    void SetA(unsigned char a) {
        SetX(a);
    }

    void SetR(unsigned char r) {
        dword = (dword & 0xFF00FFFFu) | (r << 16u);
    }

    void SetG(unsigned char g) {
        dword = (dword & 0xFFFF00FFu) | (g << 8u);
    }

    void SetB(unsigned char b) {
        dword = (dword & 0xFFFFFF00u) | b;
    }

public:
    unsigned int dword;
};

namespace Colors {
    static constexpr Color White(255u, 255u, 255u);
    static constexpr Color Black(0u, 0u, 0u);
    static constexpr Color Gray(0x80u, 0x80u, 0x80u);
    static constexpr Color LightGray(0xD3u, 0xD3u, 0xD3u);
    static constexpr Color Red(255u, 0u, 0u);
    static constexpr Color Green(0u, 255u, 0u);
    static constexpr Color Blue(0u, 0u, 255u);
    static constexpr Color Yellow(255u, 255u, 0u);
    static constexpr Color Cyan(0u, 255u, 255u);
    static constexpr Color Magenta(255u, 0u, 255u);
}  // namespace Colors
