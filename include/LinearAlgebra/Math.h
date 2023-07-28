#pragma once

#ifndef MATH_H
#define MATH_H

#include <math.h> /* sin, cos, tan, ... */
#include <cmath>
#include <limits>
#include <numbers>
#include <random>

namespace Math {
    // Constants

    template<typename T = double>
    inline constexpr T PI = std::numbers::pi_v<T>;

    template<typename T = double>
    inline constexpr T e = std::numbers::e_v<T>;

    template<typename T = double>
    inline constexpr T epsilon = std::numeric_limits<T>::epsilon();

    // Angle Functions

    template<typename T>
    inline auto toRadians(const T& degrees) {
        return degrees * (PI<T> / 180.0);
    }

    template<typename T>
    inline auto toDegrees(const T& radians) {
        return radians * (180.0 / PI<T>);
    }

    template<typename T>
    inline T wrapAngle(T theta) {
        const auto p = PI<T>;
        const T modded = fmod(theta, (T)2.0 * p);

        return (modded > p) ? (modded - (T)2.0 * p) : modded;
    }

    // Basic Functions

    template<typename T>
    inline auto square(const T& x) {
        return x * x;
    }

    template<typename T>
    inline auto cube(const T& x) {
        return x * x * x;
    }

    // Interpolation Functions

    template<typename T>
    inline auto lerp(const T& start, const T& end, const double alpha) {
        return start + (end - start) * alpha;
    }

    // Mapping Function

    template<typename T>
    inline auto map(const T& value, const T& start1, const T& stop1, const T& start2, const T& stop2) {
        return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
    }

    template<typename T>
    inline auto wrap(const T& number, const T& lower, const T& upper) {
        const T diff = upper - lower;

        while (number < lower) {
            number += diff;
        }

        while (number >= upper) {
            number -= diff;
        }

        return number;
    }

    // Other Functions

    template<typename T>
    inline T min(const T& a, const T& b) {
        return (a < b) ? a : b;
    }

    template<typename T>
    inline T max(const T& a, const T& b) {
        return (a > b) ? a : b;
    }

    // Random Number Functions

    template<typename T>
    inline T random() {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_real_distribution<T> dis(0.0, 1.0);

        return dis(gen);
    }

    template<typename T>
    inline T random(const T& min, const T& max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_real_distribution<T> dis(min, max);

        return dis(gen);
    }

    template<typename T>
    inline void setRandomSeed(unsigned int const seed) {
        std::mt19937 gen(seed);

        gen.discard(70000);  // Discard initial random values for better seed coverage
        random_engine<T>() = gen;
    }

    template<typename T>
    inline void setRandomSeed() {
        std::random_device rd;

        setRandomSeed<T>(rd());
    }

    template<typename T>
    inline std::mt19937& random_engine() {
        static std::mt19937 gen;

        return gen;
    }
}  // namespace Math

#endif  // MATH_H
