#pragma once

#include "Math.h"
#include "Matrix3.h"
#include "Vector3.h"

#include <array>
#include <cstddef>
#include <iostream>

template<typename T>
class Vector4 {
private:
    std::array<T, 4> elements = {0.0, 0.0, 0.0, 1.0f};

public:
    // Static Methods

    // Constructors

    Vector4() = default;  // Default.

    Vector4(T x, T y, T z, T w = 1.0f) : elements {x, y, z, w} {
    }  // Based on parameters.

    Vector4(const Vector3& vector, float w = 1.0f) : Vector3(vector), w(w) {
    }  // Based on Vector3 and optional w.

    Vector4(const Vector4& vector) : elements {vector[0], vector[1], vector[2], vector[3]} {
    }  // Based on other Vector4.

    // Accessor functions

    /**
     * Access the nᵗʰ scalar value in the vector.
     */
    T& operator[](std::size_t index) {
        return elements[index];
    }

    /**
     * Access the nᵗʰ scalar value in the vector as a constant expression.
     */
    T const& operator[](std::size_t index) const {
        return elements[index];
    }

    template<typename T>
    explicit operator Vector4<T>() const {
        return {(T)elements[0], (T)elements[1], (T)elements[2], (T)elements[3]};
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vector4<T>& vector) {
        stream << "[" << vector[0] << ", " << vector[1] << ", " << vector[2] << ", " << vector[3] << "]";

        return stream;
    }

    // Comparison Operators

    bool operator==(const Vector4& vector) const {
        return elements == vector.elements;
    }

    bool operator!=(const Vector4& vector) const {
        return elements != vector.elements;
    }

    // Assignment Operators

    Vector4& operator/=(const Vector4& vector) {
        elements[0] /= vector[0];
        elements[1] /= vector[1];
        elements[2] /= vector[2];
        elements[3] /= vector[3];

        return *this;
    }

    Vector4& operator/=(const T& scalar) {
        elements[0] /= scalar;
        elements[1] /= scalar;
        elements[2] /= scalar;
        elements[3] /= scalar;

        return *this;
    }

    Vector4& operator*=(const Vector4& vector) {
        elements[0] *= vector[0];
        elements[1] *= vector[1];
        elements[2] *= vector[2];
        elements[3] *= vector[2];

        return *this;
    }

    Vector4& operator*=(const T& scalar) {
        elements[0] *= scalar;
        elements[1] *= scalar;
        elements[2] *= scalar;
        elements[3] *= scalar;

        return *this;
    }

    Vector4& operator+=(const Vector4& vector) {
        elements[0] += vector[0];
        elements[1] += vector[1];
        elements[2] += vector[2];
        elements[3] += vector[2];

        return *this;
    }

    Vector4& operator-=(const Vector4& vector) {
        elements[0] -= vector[0];
        elements[1] -= vector[1];
        elements[2] -= vector[2];
        elements[3] -= vector[2];

        return *this;
    }

    // Arithmetic Operators

    Vector4 operator/(const Vector4& vector) {
        return Vector4(*this) /= vector;
    }

    Vector4 operator/(const T& vector) const {
        return Vector4(*this) /= vector;
    }

    Vector4 operator*(const Vector4& vector) {
        return Vector4(*this) *= vector;
    }

    Vector4 operator*(const T& vector) const {
        return Vector4(*this) *= vector;
    }

    Vector4 operator+(const Vector4& vector) const {
        return Vector4(*this) += vector;
    }

    Vector4 operator-(const Vector4& vector) const {
        return Vector4(*this) -= vector;
    }

    Vector4 operator-() const {
        return Vector4(-elements[0], -elements[1]);  // Negate.
    }

    // Methods

    Vector4& Negate() {
        return *this *= -1;
    }

    void Print() const {
        std::cout << "[" << elements[0] << ", " << elements[1] << ", " << elements[2] << ", " << elements[3] << "]"
                  << std::endl;
    }
};

using Vec4 = Vector4<float>;
