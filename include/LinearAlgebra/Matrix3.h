#pragma once

#include <array>    /* operator==, array */
#include <cmath>    /* cos, sin */
#include <cstddef>  /* for size_t */
#include <iostream> /* char_traits, operator<<, ostream, cout, endl */

/**
 * Class for a three-dimensional vector object and assorted functions useful for manipulation.
 */
template<typename T>
class Matrix3 {
private:
    std::array<T, 9> elements = {0, 0, 0, 0, 0, 0, 0, 0, 0};  //! std::array array{ std::array{0, 0, 0}, std::array{0,
                                                              //! 0, 0}, std::array{0, 0, 0} };

public:
    // Static Functions

    /**
     * Creates an identity matrix:
     * [1   0   0]
     * [0   1   0]
     * [0   0   1]
     * @return {Matrix3} Identity matrix.
     */
    static Matrix3 CreateIdentityMatrix() {
        return {1, 0, 0, 0, 1, 0, 0, 0, 1};
    }

    /**
     * Creates a scaling matrix:
     * [f   0   0]
     * [0   f   0]
     * [0   0   f]
     * @return {Matrix3} Scaling matrix.
     */
    static Matrix3 CreateScalingMatrix(T factor) {
        return {factor, 0, 0, 0, factor, 0, 0, 0, factor};
    }

    /**
     * Creates a rotation matrix:
     * [1      0         0  ]
     * [0    cos(θ)   sin(θ)]
     * [0   -sin(θ)   cos(θ)]
     * @return {Matrix3} Rotation matrix.
     */
    static Matrix3 CreateXRotationMatrix(T theta) {
        const T sinTheta = sin(theta);
        const T cosTheta = cos(theta);

        return {1, 0, 0, 0, cosTheta, sinTheta, 0, -sinTheta, cosTheta};
    }

    /**
     * Creates a rotation matrix:
     * [cos(θ)   0   -sin(θ)]
     * [  0      1      0   ]
     * [sin(θ)   0    cos(θ)]
     * @return {Matrix3} Rotation matrix.
     */
    static Matrix3 CreateYRotationMatrix(T theta) {
        const T sinTheta = sin(theta);
        const T cosTheta = cos(theta);

        return {cosTheta, 0, -sinTheta, 0, 1, 0, sinTheta, 0, cosTheta};
    }

    /**
     * Creates a rotation matrix:
     * [ cos(θ)   sin(θ)   0]
     * [-sin(θ)   cos(θ)   0]
     * [    0       0      1]
     * @return {Matrix3} Rotation matrix.
     */
    static Matrix3 CreateZRotationMatrix(T theta) {
        const T sinTheta = sin(theta);
        const T cosTheta = cos(theta);

        return {cosTheta, sinTheta, 0, -sinTheta, cosTheta, 0, 0, 0, 1};
    }

    // Constructors

    Matrix3() = default;  //* Default.

    Matrix3(T M00, T M01, T M02, T M10, T M11, T M12, T M20, T M21, T M22)
        : elements {M00, M01, M02, M10, M11, M12, M20, M21, M22} {  //* Based on parameters.
    }

    // Accessor Operators

    T& operator[](std::size_t index) {
        return elements[index];
    }

    T const& operator[](std::size_t index) const {
        return elements[index];
    }

    template<typename C>
    explicit operator Matrix3<C>() const {
        return {(C)elements[0],
            (C)elements[1],
            (C)elements[2],
            (C)elements[3],
            (C)elements[4],
            (C)elements[5],
            (C)elements[6],
            (C)elements[7],
            (C)elements[8]};
    }

    friend std::ostream& operator<<(std::ostream& stream, const Matrix3<T>& vector) {
        stream << "[" << vector[0] << ", " << vector[1] << ", " << vector[2] << "\n " << vector[3] << ", " << vector[4]
               << ", " << vector[5] << "\n " << vector[6] << ", " << vector[7] << ", " << vector[8] << "]";

        return stream;
    }

    // Comparison Operators

    bool operator==(const Matrix3& matrix) const {
        return elements == matrix.elements;
    }

    bool operator!=(const Matrix3& matrix) const {
        return elements != matrix.elements;
    }

    // Assignment Operators

    /**
     * Performs matrix multiplication:
     * [M00*N00 + M01*N10 + M02*N20   M00*N01 + M01*N11 + M02*N21   M00*N02 + M01*N12 + M02*N22]
     * [M10*N00 + M11*N10 + M12*N20   M10*N01 + M11*N11 + M12*N21   M10*N02 + M11*N12 + M12*N22]
     * [M20*N00 + M21*N10 + M22*N20   M20*N01 + M21*N11 + M22*N21   M20*N02 + M21*N12 + M22*N22]
     * @return {Matrix3} Matrix.
     */
    Matrix3& operator*=(const Matrix3& matrix) {  //* Matrix multiplication.
        auto [M00, M01, M02, M10, M11, M12, M20, M21, M22] = elements;
        auto [N00, N01, N02, N10, N11, N12, N20, N21, N22] = matrix.elements;

        elements[0] = M00 * N00 + M01 * N10 + M02 * N20;
        elements[1] = M00 * N01 + M01 * N11 + M02 * N21;
        elements[2] = M00 * N02 + M01 * N12 + M02 * N22;
        elements[3] = M10 * N00 + M11 * N10 + M12 * N20;
        elements[4] = M10 * N01 + M11 * N11 + M12 * N21;
        elements[5] = M10 * N02 + M11 * N12 + M12 * N22;
        elements[6] = M20 * N00 + M21 * N10 + M22 * N20;
        elements[7] = M20 * N01 + M21 * N11 + M22 * N21;
        elements[8] = M20 * N02 + M21 * N12 + M22 * N22;

        return *this;
    }

    /**
     * Performs scalar multiplication:
     * @return {Matrix3} Matrix.
     */
    Matrix3& operator*=(const T& scalar) {  //* Scalar multiplication.
        elements[0] *= scalar;
        elements[1] *= scalar;
        elements[2] *= scalar;
        elements[3] *= scalar;
        elements[4] *= scalar;
        elements[5] *= scalar;
        elements[6] *= scalar;
        elements[7] *= scalar;
        elements[8] *= scalar;

        return *this;
    }

    // Arithmetic Operators

    Matrix3 operator*(const Matrix3& matrix) const {
        return Matrix3(*this) *= matrix;
    }

    Matrix3 operator*(const T& scalar) const {
        return Matrix3(*this) *= scalar;
    }

    // Instance Methods

    /**
     * Returns the squared magnitude of the vector measured from the origin.
     * @return {number} The length of the vector, squared.
     */
    T GetDeterminant() const {
        auto [M00, M01, M02, M10, M11, M12, M20, M21, M22] = elements;

        return (M00 * (M11 * M22 - M12 * M21) - M01 * (M10 * M22 - M12 * M20) + M02 * (M10 * M21 - (M11 * M20)));
    }

    /**
     * Returns the inverse of this matrix if it exists or null if the matrix is not invertible.
     * @return {Matrix3} A new matrix which is the inverse of this matrix.
     */
    Matrix3 GetInverse() const {
        auto [M00, M01, M02, M10, M11, M12, M20, M21, M22] = elements;

        auto t00 = M22 * M11 - M21 * M12;
        auto t01 = M21 * M02 - M22 * M01;
        auto t02 = M12 * M01 - M11 * M02;
        auto d = M00 * t00 + M10 * t01 + M20 * t02;

        if (d != 0) {
            auto dI = 1 / d;

            return {t00 * dI,
                t01 * dI,
                t02 * dI,
                (M20 * M12 - M22 * M10) * dI,
                (M22 * M00 - M20 * M02) * dI,
                (M10 * M02 - M12 * M00) * dI,
                (M21 * M10 - M20 * M11) * dI,
                (M20 * M01 - M21 * M00) * dI,
                (M11 * M00 - M10 * M01) * dI};
        }

        return *this;
    }

    /**
     * Return the transpose of this matrix.  For an m-by-n matrix, the transpose
     * is the n-by-m matrix which results from turning rows into columns and columns
     * into rows
     * @return {Matrix3} A new matrix A^T.
     */
    Matrix3 GetTranspose() const {
        return {elements[0],
            elements[3],
            elements[6],
            elements[1],
            elements[4],
            elements[7],
            elements[2],
            elements[5],
            elements[8]};
    }

    Matrix3& SetIdentity() {
        return *this = Matrix3<T>::CreateIdentityMatrix();
    }

    Matrix3& Invert() {
        return *this = GetInverse();
    }

    Matrix3& Transpose() {
        return *this = GetTranspose();
    }

    void Print() const {
        std::cout << "[" << elements[0] << ", " << elements[1] << ", " << elements[2] << "\n " << elements[3] << ", "
                  << elements[4] << ", " << elements[5] << "\n " << elements[6] << ", " << elements[7] << ", "
                  << elements[8] << "]" << std::endl;
    }
};

using Mat3 = Matrix3<float>;
