#include <gtest/gtest.h>

#include "LinearAlgebra/Matrix3.h"

// Static Functions

TEST(Matrix3, StaticFunctionCreateIdentityMatrix) {
    Mat3 M = Mat3::CreateIdentityMatrix();

    for (int i = 0; i <= 8; i++) {
        switch (i) {
            case 0:
            case 4:
            case 8:
                EXPECT_EQ(M[i], 1.0f);

                break;
            default:
                EXPECT_EQ(M[i], 0);
        }
    }
}

TEST(Matrix3, StaticFunctionCreateScalingMatrix) {
    Mat3 M = Mat3::CreateScalingMatrix(2.5f);

    for (int i = 0; i <= 8; i++) {
        switch (i) {
            case 0:
            case 4:
            case 8:
                EXPECT_EQ(M[i], 2.5f);

                break;
            default:
                EXPECT_EQ(M[i], 0);
        }
    }
}

TEST(Matrix3, StaticFunctionCreateXRotationMatrix) {
    EXPECT_EQ(Mat3::CreateXRotationMatrix(1.2f),
        Mat3(1.0f, 0, 0, 0, 0.362357706f, 0.932039082f, 0, -0.932039082f, 0.362357706f));
}

TEST(Matrix3, StaticFunctionCreateYRotationMatrix) {
    EXPECT_EQ(Mat3::CreateYRotationMatrix(1.2f),
        Mat3(0.362357706f, 0, -0.932039082f, 0, 1.0f, 0, 0.932039082f, 0, 0.362357706f));
}

TEST(Matrix3, StaticFunctionCreateZRotationMatrix) {
    EXPECT_EQ(Mat3::CreateZRotationMatrix(1.2f),
        Mat3(0.362357706f, 0.932039082f, 0, -0.932039082f, 0.362357706f, 0, 0, 0, 1.0f));
}

// Constructors

TEST(Matrix3, ConstructorDefault) {
    EXPECT_EQ(Mat3(), Mat3(0, 0, 0, 0, 0, 0, 0, 0, 0));
}

TEST(Matrix3, ConstructorBasedOnParameters) {
    Mat3 M = Mat3(0, 1, 2, 3, 4, 5, 6, 7, 8);

    for (int i = 0; i <= 8; i++) {
        EXPECT_EQ(M[i], static_cast<float>(i));
    }
}

TEST(Matrix3, ConstructorBasedOnOtherMatrix3) {
    Mat3 M = Mat3::CreateIdentityMatrix();
    Mat3 N(M);

    M[0] = 2.5f;

    EXPECT_EQ(M[0], 2.5f);
    EXPECT_NE(M[0], N[0]);

    for (int i = 1; i <= 8; i++) {
        EXPECT_EQ(M[i], N[i]);
    }
}

// Accessor operators

TEST(Matrix3, AccessorOperatorAccessScalar) {
    Mat3 M = {9.0f, 0, 0, 9.1f, 9.2f, 0, 0, 0, 0};

    M[0] = 5.0f;
    M[3] = 10.0f;
    M[4] = 15.0f;

    EXPECT_EQ(M[0], 5.0f);
    EXPECT_EQ(M[3], 10.0f);
    EXPECT_EQ(M[4], 15.0f);
}

TEST(Matrix3, AccessorOperatorConstAccessScalar) {
    const Mat3 M = {9.0f, 0, 0, 9.1f, 9.2f, 0, 0, 0, 0};

    EXPECT_EQ(M[0], 9.0f);
    EXPECT_EQ(M[3], 9.1f);
    EXPECT_EQ(M[4], 9.2f);
}

TEST(Matrix3, AccessorOperatorCast) {
    // Create a Matrix3 with some test data of type float
    const Mat3 M = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};

    // Convert the Matrix3<float> to Matrix3<int>
    auto N = static_cast<Matrix3<int>>(M);

    // Verify the conversion is done correctly
    for (int i = 0; i <= 8; i++) {
        EXPECT_EQ(N[i], i);
    }
}

// Comparison Operators

TEST(Matrix3, ComparisonOperatorEquals) {
    EXPECT_TRUE(Mat3::CreateScalingMatrix(2.5f) == Mat3::CreateScalingMatrix(2.5f));
    EXPECT_FALSE(Mat3::CreateScalingMatrix(2.5f) == Mat3::CreateScalingMatrix(1.5f));
}

TEST(Matrix3, ComparisonOperatorDoesNotEqual) {
    EXPECT_TRUE(Mat3::CreateScalingMatrix(2.5f) != Mat3::CreateScalingMatrix(1.5f));
    EXPECT_FALSE(Mat3::CreateScalingMatrix(2.5f) != Mat3::CreateScalingMatrix(2.5f));
}

// Assignment Operators

TEST(Matrix3, AssignmentOperatorTimesEquals) {
    EXPECT_EQ(Mat3(1, 0, 0, 0, 0.3f, 0, 0, 0, 2) *= Mat3(), Mat3());  //* Matrix multiplication.
    EXPECT_EQ(Mat3(3, 2, 1, 6, 3, 3, 1, 4, 5) *= Mat3(3, 2, 1, 6, 3, 3, 1, 4, 5),
        Mat3(22, 16, 14, 39, 33, 30, 32, 34, 38));

    EXPECT_EQ(Mat3() *= 2, Mat3());  //* Scalar multiplication.
    EXPECT_EQ(Mat3(1, 0, 0, 0, 0.3f, 0, 0, 0, 2) *= 2, Mat3(2, 0, 0, 0, 0.6f, 0, 0, 0, 4));
}

// Arithmetic Operators

TEST(Matrix3, ArithmeticOperatorTimes) {
    EXPECT_EQ(Mat3(1, 1, 0, 0, 0.5f, 0, 0, 1, 1) * Mat3(0.5f, 0, 0, 4.5f, 0.5f, -2.25f, -1, 0, 0.5f),
        Mat3(5, 0.5f, -2.25f, 2.25f, 0.25f, -1.125f, 3.5f, 0.5f, -1.75f));  //* Matrix multiplication.

    EXPECT_EQ(Mat3::CreateScalingMatrix(1.5f) * 2,
        Mat3(3.0f, 0, 0, 0, 3.0f, 0, 0, 0, 3.0f));  //* Scalar multiplication.
}

// Instance Methods

TEST(Matrix3, InstanceMethodGetDeterminant) {
    EXPECT_EQ(Mat3().GetDeterminant(), 0);
    EXPECT_EQ(Mat3(6, 1, 1, 4, -2, 5, 2, 8, 7).GetDeterminant(), -306);
}

TEST(Matrix3, InstanceMethodGetInverse) {
    EXPECT_EQ(Mat3().GetInverse(), Mat3());
    EXPECT_EQ(Mat3(9, 3, 7, 2, 1, 0, 4, 1, 2).GetInverse(),
        Mat3(-0.25f, -0.125f, 0.875f, 0.5f, 1.25f, -1.75f, 0.25f, -0.375f, -0.375f));
    EXPECT_EQ(Mat3(2, 2, 3, 2, 1, 2, 3, 0, 1).GetInverse(), Mat3(1, -2, 1, 4, -7, 2, -3, 6, -2));
}

TEST(Matrix3, InstanceMethodGetTranspose) {
    EXPECT_EQ(Mat3(1, 0, 0, 1, 0, 0, 1, 0, 0).GetTranspose(), Mat3(1, 1, 1, 0, 0, 0, 0, 0, 0));
}

TEST(Matrix3, InstanceMethodSetIdentity) {
    EXPECT_EQ(Mat3().SetIdentity(), Mat3::CreateIdentityMatrix());
}
