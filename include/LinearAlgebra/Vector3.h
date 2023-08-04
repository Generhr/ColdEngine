#pragma once

#include <array>          /* array */
#include <iostream>       /* char_traits, operator<<, ostream, cout, endl */
#include <compare>        /* operator<, operator<=, operator>, operator>= */
#include <cstddef>        /* size_t */

#include "Math.h" /* square */

#include "Matrix3.h"

/**
 * Class for a three-dimensional vector object and assorted functions useful for manipulation.
 */
template<typename T>
class Vector3 {
private:
    std::array<T, 3> elements = {0, 0, 0};

public:
    // Static Functions

    /**
     * Returns the distance between two vectors.
     * @param {Vector3} vector1 The first vector.
     * @param {Vector3} vector2 The second vector.
     * @return {number} The distance.
     */
    constexpr static T Distance(const Vector3& vector1, const Vector3& vector2) {
        return std::sqrt(Math::square(vector1.elements[0] - vector2.elements[0]) +
                         Math::square(vector1.elements[1] - vector2.elements[1]) +
                         Math::square(vector1.elements[2] - vector2.elements[2]));
    }

    /**
     * Returns the squared distance between two vectors.
     * @param {Vector3} vector1 The first vector.
     * @param {Vector3} vector2 The second vector.
     * @return {number} The squared distance.
     */
    constexpr static T DistanceSquared(const Vector3& vector1, const Vector3& vector2) {
        return Math::square(vector1.elements[0] - vector2.elements[0]) +
               Math::square(vector1.elements[1] - vector2.elements[1]) +
               Math::square(vector1.elements[2] - vector2.elements[2]);
    }

    /**
     * Returns the dot-product of two vectors.
     * @param {Vector3} vector1 The first vector.
     * @param {Vector3} vector2 The second vector.
     * @return {number} The dot-product of the two vectors.
     */
    constexpr static T DotProduct(const Vector3& vector1, const Vector3& vector2) {
        return vector1.elements[0] * vector2.elements[0] + vector1.elements[1] * vector2.elements[1] +
               vector1.elements[2] * vector2.elements[2];
    }

    constexpr static Vector3 HadamardProduct(const Vector3& vector1, const Vector3& vector2) {
        return {vector1.elements[0] * vector2.elements[0],
            vector1.elements[1] * vector2.elements[1],
            vector1.elements[2] * vector2.elements[2]};
    }

    /**
     * Returns the determinant of two vectors.
     * @param {Vector3} vector1 The first vector.
     * @param {Vector3} vector2 The second vector.
     * @return {number} The determinant of the two vectors.
     */
    constexpr static Vector3 CrossProduct(const Vector3& vector1, const Vector3& vector2) {
        return {vector1.elements[1] * vector2.elements[2] - vector1.elements[2] * vector2.elements[1],
            vector1.elements[2] * vector2.elements[0] - vector1.elements[0] * vector2.elements[2],
            vector1.elements[0] * vector2.elements[1] - vector1.elements[1] * vector2.elements[0]};
    }

    /**
     * Returns a new Vector2 object that is the linear interpolant between vectors `vector1` and `vector2` at
     * scale-value `alpha`.
     * @param {Vector3} vector1 The first vector.
     * @param {Vector3} vector2 The second vector.
     * @param {float} alpha The proportion between the first and second vectors.
     * @return {Vector3} The interpolated vector.
     */
    constexpr static Vector3 Lerp(const Vector3& vector1, const Vector3& vector2, float alpha) {
        return {vector1.elements[0] + (vector2.elements[0] - vector1.elements[0]) * alpha,
            vector1.elements[1] + (vector2.elements[1] - vector1.elements[1]) * alpha,
            vector1.elements[2] + (vector2.elements[2] - vector1.elements[2]) * alpha};
    }

    constexpr static Vector3 Transform(const Vector3& vector, const Matrix3<T>& matrix) {  //* Matrix3 transform.
        const auto [x, y, z] = vector.elements;

        return {matrix[0] * x + matrix[3] * y + matrix[6] * z,
            matrix[1] * x + matrix[4] * y + matrix[7] * z,
            matrix[2] * x + matrix[5] * y + matrix[8] * z};
    }

    // Constructors

    Vector3() = default;  //* Default.

    Vector3(T x, T y, T z) : elements {x, y, z} {  //* Based on parameters.
    }

    /**
     * Returns a new Vector3 object from a given vector.
     * @param {Vector3} vector The vector.
     * @return {Vector3} A new vector object.
     */
    Vector3(const Vector3& vector) : elements {vector[0], vector[1], vector[2]} {  //* Based on other Vector3.
    }

    // Accessor Operators

    /**
     * Access the nᵗʰ scalar value in the vector.
     */
    T& operator[](const std::size_t index) {
        return elements[index];
    }

    /**
     * Access the nᵗʰ scalar value in the vector as a constant expression.
     */
    T const& operator[](const std::size_t index) const {
        return elements[index];
    }

    template<typename U>
    explicit operator Vector3<U>() const {
        return {static_cast<U>(elements[0]), static_cast<U>(elements[1]), static_cast<U>(elements[2])};
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vector3<T>& vector) {
        stream << "[" << vector[0] << ", " << vector[1] << ", " << vector[2] << "]";

        return stream;
    }

    // Comparison Operators

    bool operator==(const Vector3& vector) const {
        return elements == vector.elements;
    }

    bool operator!=(const Vector3& vector) const {
        return elements != vector.elements;
    }

    bool operator<(const Vector3& vector) const {
        return elements < vector.elements;
    }

    bool operator<=(const Vector3& vector) const {
        return elements <= vector.elements;
    }

    bool operator>(const Vector3& vector) const {
        return elements > vector.elements;
    }

    bool operator>=(const Vector3& vector) const {
        return elements >= vector.elements;
    }

    // Assignment Operators

    Vector3& operator/=(const T& scalar) {
        elements[0] /= scalar;
        elements[1] /= scalar;
        elements[2] /= scalar;

        return *this;
    }

    Vector3& operator*=(const T& scalar) {
        elements[0] *= scalar;
        elements[1] *= scalar;
        elements[2] *= scalar;

        return *this;
    }

    Vector3& operator*=(const Matrix3<T>& matrix) {  //* Matrix3 transform.
        const auto [x, y, z] = elements;

        elements[0] = matrix[0] * x + matrix[3] * y + matrix[6] * z;
        elements[1] = matrix[1] * x + matrix[4] * y + matrix[7] * z;
        elements[2] = matrix[2] * x + matrix[5] * y + matrix[8] * z;

        return *this;
    }

    Vector3& operator+=(const Vector3& vector) {
        elements[0] += vector[0];
        elements[1] += vector[1];
        elements[2] += vector[2];

        return *this;
    }

    Vector3& operator-=(const Vector3& vector) {
        elements[0] -= vector[0];
        elements[1] -= vector[1];
        elements[2] -= vector[2];

        return *this;
    }

    // Arithmetic Operators

    Vector3 operator/(const T& scalar) const {
        return Vector3(*this) /= scalar;
    }

    Vector3 operator*(const T& scalar) const {
        return Vector3(*this) *= scalar;
    }

    T operator*(const Vector3& vector) const {  //* Dot product.
        return elements[0] * vector[0] + elements[1] * vector[1] + elements[2] * vector[2];
    }

    Vector3 operator*(const Matrix3<T> matrix) const {  //* Matrix3 transform.
        return Vector3<T>(*this) *= matrix;
    }

    Vector3 operator+(const Vector3& vector) const {
        return Vector3(*this) += vector;
    }

    Vector3 operator-(const Vector3& vector) const {
        return Vector3(*this) -= vector;
    }

    Vector3 operator-() const {  //* Negate.
        return {-elements[0], -elements[1], -elements[2]};
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
    Vector3 GetNormalized() const {
        const T magnitude = GetMagnitude();

        if (magnitude != 0.0f) {
            return Vector3(*this) /= magnitude;
        }

        return *this;
    }

    /**
     * Normalizes the current vector to have a magnitude of 1.
     * @return {Vector3} The normalized vector.
     */
    Vector3& Normalize() {
        return *this = GetNormalized();
    }

    /**
     * Reverses the sign of the vector. Equivalent to scaling the vector by -1.
     * @return {Vector3} The inverted vector.
     */
    Vector3& Negate() {
        return *this *= -1;
    }

    /**
     * Sets this vector to the linear interpolant between itself and another vector at scale-value `alpha`.
     * @param {Vector3} a Vector a.
     * @param {Vector3} b Vector b.
     * @param {number} alpha The proportion between a and b.
     * @return {Vector3} The interpolated vector.
     */
    Vector3& Lerp(const Vector3& vector, float alpha) {
        elements[0] += (vector[0] - elements[0]) * alpha;
        elements[1] += (vector[1] - elements[1]) * alpha;
        elements[2] += (vector[2] - elements[2]) * alpha;

        return *this;
    }

    void Print() const {
        std::cout << "[" << elements[0] << ", " << elements[1] << ", " << elements[2] << "]" << std::endl;
    }
};

using Vec3 = Vector3<float>;
