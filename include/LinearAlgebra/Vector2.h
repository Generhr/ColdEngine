#pragma once

#include "Math.h"
#include "Matrix2.h"
#include "Matrix3.h"

#include <array>    /* array */
#include <iostream> /* stream, cout */

/**
 * Class for a two-dimensional vector object and assorted functions useful for manipulating points.
 */
template<typename T>
class Vector2 {
private:
    std::array<T, 2> elements = {0, 0};

public:
    // Static Functions

    /**
     * Returns the distance between two vectors.
     * @param {Vector2} vector1 The first vector.
     * @param {Vector2} vector2 The second vector.
     * @return {number} The distance.
     */
    constexpr static T Distance(const Vector2& vector1, const Vector2& vector2) {
        return std::sqrt(Math::square(vector1.elements[0] - vector2.elements[0]) +
                         Math::square(vector1.elements[1] - vector2.elements[1]));
    }

    /**
     * Returns the squared distance between two vectors.
     * @param {Vector2} vector1 The first vector.
     * @param {Vector2} vector2 The second vector.
     * @return {number} The squared distance.
     */
    constexpr static T DistanceSquared(const Vector2& vector1, const Vector2& vector2) {
        return Math::square(vector1.elements[0] - vector2.elements[0]) +
               Math::square(vector1.elements[1] - vector2.elements[1]);
    }

    /**
     * Returns the dot-product of two vectors.
     * @param {Vector2} vector1 The first vector.
     * @param {Vector2} vector2 The second vector.
     * @return {number} The dot-product of the two vectors.
     */
    constexpr static T DotProduct(const Vector2& vector1, const Vector2& vector2) {
        return vector1.elements[0] * vector2.elements[0] + vector1.elements[1] * vector2.elements[1];
    }

    constexpr static Vector2 HadamardProduct(const Vector2& vector1, const Vector2& vector2) {
        return {vector1.elements[0] * vector2.elements[0], vector1.elements[1] * vector2.elements[1]};
    }

    /**
     * Returns the determinant of two vectors.
     * @param {Vector2} vector1 The first vector.
     * @param {Vector2} vector2 The second vector.
     * @return {number} The determinant of the two vectors.
     */
    constexpr static T CrossProduct(const Vector2& vector1, const Vector2& vector2) {
        return vector1.elements[0] * vector2.elements[1] - vector1.elements[1] * vector2.elements[0];
    }

    /**
     * Returns a new Vector2 object that is the linear interpolant between vectors `vector1` and `vector2` at
     * scale-value `alpha`.
     * @param {Vector2} vector1 The first vector.
     * @param {Vector2} vector2 The second vector.
     * @param {float} alpha The proportion between the first and second vectors.
     * @return {Vector2} The interpolated vector.
     */
    constexpr static Vector2 Lerp(const Vector2& vector1, const Vector2& vector2, float alpha) {
        return {vector1.elements[0] + (vector2.elements[0] - vector1.elements[0]) * alpha,
            vector1.elements[1] + (vector2.elements[1] - vector1.elements[1]) * alpha};
    }

    constexpr static Vector2 Transform(const Vector2& vector, const Matrix2<T>& matrix) {  //* Matrix2 transform.
        //         [m00   m01]
        // [x   y] [m10   m11]

        return {vector.elements[0] * matrix[0] + vector.elements[1] * matrix[2],
            vector.elements[0] * matrix[1] + vector.elements[1] * matrix[3]};
    }

    constexpr static Vector2 Transform(const Vector2& vector, const Matrix3<T>& matrix) {  //* Matrix3 transform.
        //             [m00   m01   0]
        // [x   y   1] [m10   m11   0]
        //             [m20   m21   1]

        return {vector.elements[0] * matrix[0] + vector.elements[1] * matrix[3] + matrix[6],
            vector.elements[0] * matrix[1] + vector.elements[1] * matrix[4] + matrix[7]};
    }

    // Constructors

    Vector2() = default;  //* Default.

    Vector2(T x, T y) : elements {x, y} {  //* Based on parameters.
    }

    Vector2(const Vector2& vector) : elements {vector.elements[0], vector.elements[1]} {  //* Based on other Vector2.
    }

    // Accessor Operators

    T& operator[](const std::size_t index) {
        return elements[index];
    }

    T const& operator[](const std::size_t index) const {
        return elements[index];
    }

    template<typename T>
    explicit operator Vector2<T>() const {
        return {(T)elements[0], (T)elements[1]};
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vector2<T>& vector) {
        stream << "[" << vector.elements[0] << ", " << vector.elements[1] << "]";

        return stream;
    }

    // Comparison Operators

    bool operator==(const Vector2& vector) const {
        return elements == vector.elements;
    }

    bool operator!=(const Vector2& vector) const {
        return elements != vector.elements;
    }

    bool operator<(const Vector2& vector) const {
        return elements < vector.elements;
    }

    bool operator<=(const Vector2& vector) const {
        return elements <= vector.elements;
    }

    bool operator>(const Vector2& vector) const {
        return elements > vector.elements;
    }

    bool operator>=(const Vector2& vector) const {
        return elements >= vector.elements;
    }

    // Assignment Operators

    Vector2& operator/=(const T& scalar) {
        elements[0] /= scalar;
        elements[1] /= scalar;

        return *this;
    }

    Vector2& operator*=(const T& scalar) {
        elements[0] *= scalar;
        elements[1] *= scalar;

        return *this;
    }

    Vector2& operator*=(const Matrix2<T>& matrix) {  //* Matrix2 transform.
        const auto [x, y] = elements;

        elements[0] = matrix[0] * x + matrix[2] * y;
        elements[1] = matrix[1] * x + matrix[3] * y;

        return *this;
    }

    Vector2& operator*=(const Matrix3<T>& matrix) {  //* Matrix3 transform.
        const auto [x, y] = elements;

        elements[0] = matrix[0] * x + matrix[3] * y + matrix[2];
        elements[1] = matrix[1] * x + matrix[4] * y + matrix[5];

        return *this;
    }

    Vector2& operator+=(const Vector2& vector) {
        elements[0] += vector.elements[0];
        elements[1] += vector.elements[1];

        return *this;
    }

    Vector2& operator-=(const Vector2& vector) {
        elements[0] -= vector.elements[0];
        elements[1] -= vector.elements[1];

        return *this;
    }

    // Arithmetic Operators

    Vector2 operator/(const T& scalar) const {
        return Vector2(*this) /= scalar;
    }

    Vector2 operator*(const T& scalar) const {
        return Vector2(*this) *= scalar;
    }

    T operator*(const Vector2& vector) const {  //* Dot product.
        return elements[0] * vector.elements[0] + elements[1] * vector.elements[1];
    }

    Vector2 operator*(const Matrix2<T> matrix) const {  //* Matrix2 transform.
        return Vector2<T>(*this) *= matrix;
    }

    Vector2 operator*(const Matrix3<T> matrix) const {  //* Matrix3 transform.
        return Vector2<T>(*this) *= matrix;
    }

    Vector2 operator+(const Vector2& vector) const {
        return Vector2(*this) += vector;
    }

    Vector2 operator-(const Vector2& vector) const {
        return Vector2(*this) -= vector;
    }

    Vector2 operator-() const {  //* Negate.
        return {-elements[0], -elements[1]};
    }

    // Instance Methods

    /**
     * Returns the squared magnitude of the vector measured from the origin.
     * @return {number} The length of the vector, squared.
     */
    T GetMagnitudeSquared() const {
        return Math::square(*this);
    }

    /**
     * Returns the magnitude of the vector measured from the origin.
     * @return {number} The length of the vector.
     */
    T GetMagnitude() const {
        return std::sqrt(GetMagnitudeSquared());
    }

    /**
     * Returns the squared magnitude of this vector measured from the origin.
     * @return {number} The length of the vector, squared.
     */
    Vector2 GetNormalized() const {
        const T magnitude = GetMagnitude();

        if (magnitude != 0.0f) {
            return Vector2(*this) /= magnitude;
        }

        return *this;
    }

    /**
     * Normalizes the current vector to have a magnitude of 1.
     * @return {Vector2} The normalized vector.
     */
    Vector2& Normalize() {
        return *this = GetNormalized();
    }

    /**
     * Reverses the sign of the vector. Equivalent to scaling the vector by -1.
     * @return {Vector2} The inverted vector.
     */
    Vector2& Negate() {
        return *this *= -1;
    }

    /**
     * Rotates this vector in-place by a given angle, specified in radians.
     * @param {number} theta The angle, in radians.
     * @return {Vector2} This vector rotated `theta` radians.
     */
    Vector2& Rotate(const T& theta) {
        const auto [x, y] = elements;

        const auto c = cos(theta);
        const auto s = sin(theta);

        elements[0] = x * c - y * s;
        elements[1] = y * c + x * s;

        return *this;
    }

    /**
     * Rotates a vector by a given angle, specified in radians, relative to a given axis rotation point.
     * @param {Vector2} vector The rotation axis point.
     * @param {number} theta The angle, in radians.
     * @return {Vector2} This vector rotated `theta` radians.
     */
    Vector2& Rotate(const Vector2& vector, const T& theta) {  //* Rotate around point.
        const auto [x2, y2] = vector.elements;
        const auto x1 = elements[0] - x2;
        const auto y1 = elements[1] - y2;

        const auto c = cos(theta);
        const auto s = sin(theta);

        elements[0] = (x1 * c - y1 * s) + x2;
        elements[1] = (y1 * c + x1 * s) + y2;

        return *this;
    }

    /**
     * Sets this vector to the linear interpolant between itself and another vector at scale-value `alpha`.
     * @param {Vector2} a Vector a.
     * @param {Vector2} b Vector b.
     * @param {number} alpha The proportion between a and b.
     * @return {Vector2} The interpolated vector.
     */
    Vector2& Lerp(const Vector2& vector, float alpha) {
        elements[0] += (vector.elements[0] - elements[0]) * alpha;
        elements[1] += (vector.elements[1] - elements[1]) * alpha;

        return *this;
    }

    void Print() const {
        std::cout << "[" << elements[0] << ", " << elements[1] << "]" << std::endl;
    }
};

using Vec2 = Vector2<float>;
