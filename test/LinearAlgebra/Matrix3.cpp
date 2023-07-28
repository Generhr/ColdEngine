#include "LinearAlgebra/Matrix3.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <initializer_list>

// Static Functions

TEST(Matrix3, StaticFunctionCreateIdentityMatrix) {
    Mat3 M = Mat3::CreateIdentityMatrix();

    for (int i = 0; i <= 8; i++) {
        switch (i) {
            case 0:
            case 4:
            case 8:
                EXPECT_EQ(M[i], 1.0F);

                break;
            default:
                EXPECT_EQ(M[i], 0);
        }
    }
}

TEST(Matrix3, StaticFunctionCreateScalingMatrix) {
    Mat3 M = Mat3::CreateScalingMatrix(2.5F);

    for (int i = 0; i <= 8; i++) {
        switch (i) {
            case 0:
            case 4:
            case 8:
                EXPECT_EQ(M[i], 2.5F);

                break;
            default:
                EXPECT_EQ(M[i], 0);
        }
    }
}

TEST(Matrix3, StaticFunctionCreateXRotationMatrix) {
    EXPECT_EQ(Mat3::CreateXRotationMatrix(1.2F),
        Mat3(1.0F, 0, 0, 0, 0.362357706F, 0.932039082F, 0, -0.932039082F, 0.362357706F));
}

TEST(Matrix3, StaticFunctionCreateYRotationMatrix) {
    EXPECT_EQ(Mat3::CreateYRotationMatrix(1.2F),
        Mat3(0.362357706F, 0, -0.932039082F, 0, 1.0F, 0, 0.932039082F, 0, 0.362357706F));
}

TEST(Matrix3, StaticFunctionCreateZRotationMatrix) {
    EXPECT_EQ(Mat3::CreateZRotationMatrix(1.2F),
        Mat3(0.362357706F, 0.932039082F, 0, -0.932039082F, 0.362357706F, 0, 0, 0, 1.0F));
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

    M[0] = 2.5F;

    EXPECT_EQ(M[0], 2.5F);
    EXPECT_NE(M[0], N[0]);

    for (int i = 1; i <= 8; i++) {
        EXPECT_EQ(M[i], N[i]);
    }
}

// Accessor operators

TEST(Matrix3, AccessorOperatorAccessScalar) {
    Mat3 M = {9.0F, 0, 0, 9.1F, 9.2F, 0, 0, 0, 0};

    M[0] = 5.0F;
    M[3] = 10.0F;
    M[4] = 15.0F;

    EXPECT_EQ(M[0], 5.0F);
    EXPECT_EQ(M[3], 10.0F);
    EXPECT_EQ(M[4], 15.0F);
}

TEST(Matrix3, AccessorOperatorConstAccessScalar) {
    const Mat3 M = {9.0F, 0, 0, 9.1F, 9.2F, 0, 0, 0, 0};

    EXPECT_EQ(M[0], 9.0F);
    EXPECT_EQ(M[3], 9.1F);
    EXPECT_EQ(M[4], 9.2F);
}

TEST(Matrix3, AccessorOperatorCast) {
    // Create a Matrix3 with some test data of type float
    const Mat3 M = {0.0F, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F};

    // Convert the Matrix3<float> to Matrix3<int>
    auto N = static_cast<Matrix3<int>>(M);

    // Verify the conversion is done correctly
    for (int i = 0; i <= 8; i++) {
        EXPECT_EQ(N[i], i);
    }
}

// Comparison Operators

TEST(Matrix3, ComparisonOperatorEquals) {
    EXPECT_TRUE(Mat3::CreateScalingMatrix(2.5F) == Mat3::CreateScalingMatrix(2.5F));
    EXPECT_FALSE(Mat3::CreateScalingMatrix(2.5F) == Mat3::CreateScalingMatrix(1.5F));
}

TEST(Matrix3, ComparisonOperatorDoesNotEqual) {
    EXPECT_TRUE(Mat3::CreateScalingMatrix(2.5F) != Mat3::CreateScalingMatrix(1.5F));
    EXPECT_FALSE(Mat3::CreateScalingMatrix(2.5F) != Mat3::CreateScalingMatrix(2.5F));
}

// Assignment Operators

TEST(Matrix3, AssignmentOperatorTimesEquals) {
    EXPECT_EQ(Mat3(1, 0, 0, 0, 0.3F, 0, 0, 0, 2) *= Mat3(), Mat3());  //* Matrix multiplication.
    EXPECT_EQ(Mat3(3, 2, 1, 6, 3, 3, 1, 4, 5) *= Mat3(3, 2, 1, 6, 3, 3, 1, 4, 5),
        Mat3(22, 16, 14, 39, 33, 30, 32, 34, 38));

    EXPECT_EQ(Mat3() *= 2, Mat3());  //* Scalar multiplication.
    EXPECT_EQ(Mat3(1, 0, 0, 0, 0.3F, 0, 0, 0, 2) *= 2, Mat3(2, 0, 0, 0, 0.6F, 0, 0, 0, 4));
}

// Arithmetic Operators

TEST(Matrix3, ArithmeticOperatorTimes) {
    EXPECT_EQ(Mat3(1, 1, 0, 0, 0.5F, 0, 0, 1, 1) * Mat3(0.5F, 0, 0, 4.5F, 0.5F, -2.25F, -1, 0, 0.5F),
        Mat3(5, 0.5F, -2.25F, 2.25F, 0.25F, -1.125F, 3.5F, 0.5F, -1.75F));  //* Matrix multiplication.

    EXPECT_EQ(Mat3::CreateScalingMatrix(1.5F) * 2,
        Mat3(3.0F, 0, 0, 0, 3.0F, 0, 0, 0, 3.0F));  //* Scalar multiplication.
}

// Instance Methods

TEST(Matrix3, InstanceMethodGetDeterminant) {
    EXPECT_EQ(Mat3().GetDeterminant(), 0);
    EXPECT_EQ(Mat3(6, 1, 1, 4, -2, 5, 2, 8, 7).GetDeterminant(), -306);
}

TEST(Matrix3, InstanceMethodGetInverse) {
    EXPECT_EQ(Mat3().GetInverse(), Mat3());
    EXPECT_EQ(Mat3(9, 3, 7, 2, 1, 0, 4, 1, 2).GetInverse(),
        Mat3(-0.25F, -0.125F, 0.875F, 0.5F, 1.25F, -1.75F, 0.25F, -0.375F, -0.375F));
    EXPECT_EQ(Mat3(2, 2, 3, 2, 1, 2, 3, 0, 1).GetInverse(), Mat3(1, -2, 1, 4, -7, 2, -3, 6, -2));
}

TEST(Matrix3, InstanceMethodGetTranspose) {
    EXPECT_EQ(Mat3(1, 0, 0, 1, 0, 0, 1, 0, 0).GetTranspose(), Mat3(1, 1, 1, 0, 0, 0, 0, 0, 0));
}

TEST(Matrix3, InstanceMethodSetIdentity) {
    EXPECT_EQ(Mat3().SetIdentity(), Mat3::CreateIdentityMatrix());
}
