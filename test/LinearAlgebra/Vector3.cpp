#include <cmath>  /* sqrt */
#include <gtest/gtest.h>

#include "LinearAlgebra/Vector3.h"

#include "LinearAlgebra/Matrix3.h"

// Static Functions

TEST(Vector3, StaticFunctionDistance) {
    EXPECT_EQ(Vec3::Distance(Vec3(3, 4, 5), Vec3(-3, -4, 5)), 10);
}

TEST(Vector3, StaticFunctionDistanceSquared) {
    EXPECT_EQ(Vec3::DistanceSquared(Vec3(3, 4, 5), Vec3(-3, -4, 5)), 100);
}

TEST(Vector3, StaticFunctionDotProduct) {
    EXPECT_EQ(Vec3::DotProduct(Vec3(0, 5, 2), Vec3(3, 0, 5)), 10);
    EXPECT_EQ(Vec3::DotProduct(Vec3(-5, -5, 5), Vec3(0, 7, -2)), -45);
}

TEST(Vector3, StaticFunctionHadamardProduct) {
}

TEST(Vector3, StaticFunctionCrossProduct) {
    EXPECT_EQ(Vec3::CrossProduct(Vec3(3, 0, 0), Vec3(0, 2, 0)), Vec3(0, 0, 6));
    EXPECT_EQ(Vec3::CrossProduct(Vec3(1, 2, 3), Vec3(4, 5, 6)), Vec3(-3, 6, -3));
}

TEST(Vector3, StaticFunctionLerp) {
    const Vec3 a(0, 0, 0);
    const Vec3 b(10, 10, 10);

    for (int i = 0; i <= 10; i++) {
        EXPECT_EQ(Vec3::Lerp(a, b, i / 10.0f), Vec3(i, i, i));
    }
}

TEST(Vector3, StaticFunctionTransform) {
    Vec3 a = Vec3::Transform(Vec3(2, 1, -3), Mat3(1.29f, 1, 0, 0, 0.73f, 0, 0, 0, 2.34f));  //* Matrix3 transform.

    EXPECT_NEAR(a[0], 2.58f, 1e-6);
    EXPECT_NEAR(a[1], 2.73f, 1e-6);
    EXPECT_NEAR(a[2], -7.02f, 1e-6);
}

// Constructors

TEST(Vector3, ConstructorDefault) {
    Vector3<float> a;

    EXPECT_EQ(a[0], 0);
    EXPECT_EQ(a[1], 0);
    EXPECT_EQ(a[2], 0);
}

TEST(Vector3, ConstructorBasedOnParameters) {
    Vec3 a(1, 9, 5);

    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 9);
    EXPECT_EQ(a[2], 5);
}

TEST(Vector3, ConstructorBasedOnOtherVector3) {
    const Vec3 a;
    const Vec3 b;
    const Vec3 c;
    Vector3<Vec3> d(a, b, c);
    auto e = d;

    d[0][0] = 1;

    EXPECT_EQ(d[0][0], 1);
    EXPECT_NE(d[0][0], e[0][0]);
}

// Accessor operators

TEST(Vector3, AccessorOperatorAccessScalar) {
    Vec3 a(1, 5, 2);

    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 5);
    EXPECT_EQ(a[2], 2);
}

TEST(Vector3, AccessorOperatorConstAccessScalar) {
    Vec3 a(1, 8, 2);

    const float x = a[0];
    const float y = a[1];
    const float z = a[2];

    EXPECT_EQ(a[0], x);
    EXPECT_EQ(a[1], y);
    EXPECT_EQ(a[2], z);
}

TEST(Vector3, AccessorOperatorCast) {
    Vector3<int> a(Vector3<float>(1.9f, 2.3f, 5.3f));

    EXPECT_EQ(a[0], (int)1);
    EXPECT_EQ(a[1], (int)2);
    EXPECT_EQ(a[2], (int)5);
}

// Comparison Operators

TEST(Vector3, ComparisonOperatorEquals) {
    EXPECT_TRUE(Vec3(50, 50, 50) == Vec3(50, 50, 50));
    EXPECT_FALSE(Vec3(50, 50, 50) == Vec3(5, 5, 5));
}

TEST(Vector3, ComparisonOperatorDoesNotEqual) {
    EXPECT_TRUE(Vec3(5, 5, 5) != Vec3(50, 50, 50));
    EXPECT_FALSE(Vec3(5, 5, 5) != Vec3(5, 5, 5));
}

TEST(Vector3, ComparisonOperatorLessThan) {
    EXPECT_TRUE(Vec3(5, 5, 5) < Vec3(50, 50, 50));
    EXPECT_FALSE(Vec3(50, 50, 50) < Vec3(5, 5, 5));
}

TEST(Vector3, ComparisonOperatorLessThanOrEqual) {
    EXPECT_TRUE(Vec3(5, 5, 5) <= Vec3(5, 5, 5));
    EXPECT_FALSE(Vec3(50, 50, 50) <= Vec3(5, 5, 5));
}

TEST(Vector3, ComparisonOperatorGreaterThan) {
    EXPECT_TRUE(Vec3(50, 50, 50) > Vec3(5, 5, 5));
    EXPECT_FALSE(Vec3(5, 5, 5) > Vec3(50, 50, 50));
}

TEST(Vector3, ComparisonOperatorGreaterThanOrEqual) {
    EXPECT_TRUE(Vec3(50, 50, 50) >= Vec3(50, 50, 50));
    EXPECT_FALSE(Vec3(5, 5, 5) >= Vec3(50, 50, 50));
}

// Assignment Operators

TEST(Vector3, AssignmentOperatorDivideEquals) {
    EXPECT_EQ(Vec3(2.0f, 4.0f, 8.0f) /= 8.0f, Vec3(2.0f / 8.0f, 4.0f / 8.0f, 1.0f));
}

TEST(Vector3, AssignmentOperatorTimesEquals) {
    EXPECT_EQ(Vec3(2.0f, 4.0f, 8.0f) *= 8.0f, Vec3(16.0f, 32.0f, 64.0f));

    //* Matrix3 transform.
}

TEST(Vector3, AssignmentOperatorPlusEquals) {
    EXPECT_EQ(Vec3(2.0f, 4.0f, 8.0f) += Vec3(1.0f, -1.0f, 0.0f), Vec3(3.0f, 3.0f, 8.0f));
}

TEST(Vector3, AssignmentOperatorMinusEquals) {
    EXPECT_EQ(Vec3(2.0f, 4.0f, 8.0f) -= Vec3(1.0f, -1.0f, 0.0f), Vec3(1.0f, 5.0f, 8.0f));
}

// Arithmetic Operators

TEST(Vector3, ArithmeticOperatorDivide) {
    EXPECT_EQ(Vec3(2.0f, 4.0f, 8.0f) / 8.0f, Vec3(2.0f / 8.0f, 4.0f / 8.0f, 1.0f));
}

TEST(Vector3, ArithmeticOperatorTimes) {
    EXPECT_EQ(Vec3(2.0f, 4.0f, 8.0f) * 8.0f, Vec3(16.0f, 32.0f, 64.0f));

    EXPECT_EQ(Vec3(0, 5, 2) * Vec3(3, 0, 5), 10);  //* Dot product.
    EXPECT_EQ(Vec3(-5, -5, 5) * Vec3(0, 7, -2), -45);

    //* Matrix3 transform.
}

TEST(Vector3, ArithmeticOperatorPlus) {
    EXPECT_EQ(Vec3(2.0f, 4.0f, 8.0f) + Vec3(1.0f, -1.0f, 0.0f), Vec3(3.0f, 3.0f, 8.0f));
}

TEST(Vector3, ArithmeticOperatorMinus) {
    EXPECT_EQ(Vec3(2.0f, 4.0f, 8.0f) - Vec3(1.0f, -1.0f, 0.0f), Vec3(1.0f, 5.0f, 8.0f));
}

TEST(Vector3, ArithmeticOperatorNegate) {
    EXPECT_EQ(-Vec3(2.0f, 4.0f, -8.0f), Vec3(-2.0f, -4.0f, 8.0f));
    EXPECT_NE(-Vec3(2.0f, 4.0f, -8.0f), -Vec3(-2.0f, -4.0f, 8.0f));
}

// Instance Methods

TEST(Vector3, InstanceMethodGetMagnitudeSquared) {
    EXPECT_EQ(Vec3(-3, -4, -5).GetMagnitudeSquared(), 50);
}

TEST(Vector3, InstanceMethodGetMagnitude) {
    EXPECT_EQ(Vec3(0, 10, 0).GetMagnitude(), 10);
    EXPECT_NEAR(Vec3(3, 4, 5).GetMagnitude(), std::sqrt(50), 1e-6);
    EXPECT_NEAR(Vec3(-4, 3, 8).GetMagnitude(), std::sqrt(89), 1e-6);
}

TEST(Vector3, InstanceMethodGetNormalized) {
    Vec3 a(5, 5, 5);
    a = a.GetNormalized();

    EXPECT_NEAR(a[0], 0.577350f, 1e-6);
    EXPECT_NEAR(a[1], 0.577350f, 1e-6);
    EXPECT_NEAR(a[2], 0.577350f, 1e-6);
}

TEST(Vector3, InstanceMethodNormalize) {
    Vec3 a(3, 1, 2);
    a.Normalize();

    EXPECT_NEAR(a[0], 0.801784f, 1e-6);
    EXPECT_NEAR(a[1], 0.267261f, 1e-6);
    EXPECT_NEAR(a[2], 0.534522f, 1e-6);

    // EXPECT_FLOAT_EQ()
}

TEST(Vector3, InstanceMethodNegate) {
    EXPECT_EQ(Vec3(1, 15, 5).Negate(), Vec3(-1, -15, -5));
}

TEST(Vector3, InstanceMethodLerp) {
    EXPECT_EQ(Vec3(1, 2, 0).Lerp(Vec3(2, 1, 1), 0.25f), Vec3(1.25f, 1.75f, 0.25f));
}

TEST(Vector3, InstanceMethodPrint) {
}
