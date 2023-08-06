#pragma once

#include <iostream> /* stream, cout */


/**
 * Class for a three-dimensional vector object and assorted functions useful for manipulation.
 */
template<typename T>
class Matrix2 {
private:
    std::array<T, 4> elements = {0.0, 0.0, 0.0, 0.0};

public:
    // Static Functions

    constexpr static Matrix2 CreateIdentityMatrix() {
        return Matrix2<T>(1.0, 0.0, 0.0, 1.0);
    }

    constexpr static Matrix2 CreateScalingMatrix(T factor) {
        return {factor, 0.0, 0.0, factor};
    }

    constexpr static Matrix2 CreateRotationMatrix(T theta) {
        const T cosTheta = cos(theta);
        const T sinTheta = sin(theta);

        return {cosTheta, sinTheta, -sinTheta, cosTheta};
    }

    // Constructors

    Matrix2() = default;  //* Default.

    Matrix2(T m00, T m01, T m10, T m11) : elements {m00, m01, m10, m11} {  //* Based on other Matrix2.
    }

    // Accessor Operators

    T& operator[](std::size_t index) {
        return elements[index];
    }

    T const& operator[](std::size_t index) const {
        return elements[index];
    }

    template<typename C>
    explicit operator Matrix2<C>() const {
        return {(C)elements[0], (C)elements[1], (C)elements[2], (C)elements[3]};
    }

    friend std::ostream& operator<<(std::ostream& stream, const Matrix2<T>& vector) {
        stream << "[" << vector[0] << ", " << vector[1] << "\n " << vector[2] << ", " << vector[3] << "]";

        return stream;
    }

    // Comparison Operators

    bool operator==(const Matrix2& matrix) const {
        return elements == matrix.elements;
    }

    bool operator!=(const Matrix2& matrix) const {
        return elements != matrix.elements;
    }

    // Assignment Operators

    Matrix2& operator*=(const Matrix2& matrix) {  //~ FIX
        const auto [a00, a01, a10, a11] = elements;
        const auto [b00, b01, b10, b11] = matrix.elements;

        return *this;
    }

    Matrix2& operator*=(const T& scalar) {
        elements[0] *= scalar;
        elements[1] *= scalar;
        elements[2] *= scalar;
        elements[3] *= scalar;

        return *this;
    }

    // Arithmetic Operators

    Matrix2 operator*(const Matrix2& matrix) const {
        return Matrix2(*this) *= matrix;
    }

    Matrix2 operator*(const T& scalar) const {
        return Matrix2(*this) *= scalar;
    }

    // Instance Methods

    Matrix2<T>& SetIdentity() {
        return *this = Matrix2<T>::CreateIdentityMatrix();
    }

    void Print() const {
        std::cout << "[" << elements[0] << ", " << elements[1] << "\n " << elements[2] << ", " << elements[3] << "]"
                  << std::endl;
    }
};

using Mat2 = Matrix2<float>;
