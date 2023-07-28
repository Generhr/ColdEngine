#include "LinearAlgebra/Vector2.h"

#include <gtest/gtest.h>

// Static Functions

TEST(Vector2, StaticFunctionDistance) {
    EXPECT_EQ(Vec2::Distance(Vec2(3, 4), Vec2(-3, -4)), 10);
}

TEST(Vector2, StaticFunctionDistanceSquared) {
    EXPECT_EQ(Vec2::DistanceSquared(Vec2(3, 4), Vec2(-3, -4)), 100);
}

TEST(Vector2, StaticFunctionDotProduct) {
    EXPECT_EQ(Vec2::DotProduct(Vec2(0, 5), Vec2(3, 0)), 0);
    EXPECT_EQ(Vec2::DotProduct(Vec2(-5, -5), Vec2(0, 7)), -35);
}

TEST(Vector2, StaticFunctionHadamardProduct) {
}

TEST(Vector2, StaticFunctionCrossProduct) {
    EXPECT_EQ(Vec2::CrossProduct(Vec2(0, 5), Vec2(0, 10)), 0);
    EXPECT_EQ(Vec2::CrossProduct(Vec2(0, 5), Vec2(10, 0)), -50);
    EXPECT_EQ(Vec2::CrossProduct(Vec2(-5, -5), Vec2(0, 7)), -35);
}

TEST(Vector2, StaticFunctionLerp) {
    const Vec2 a(0, 0);
    const Vec2 b(10, 10);

    for (int i = 0; i <= 10; i++) {
        EXPECT_EQ(Vec2::Lerp(a, b, i / 10.0F), Vec2(i, i));
    }
}

TEST(Vector2, StaticFunctionTransform) {
    //* Matrix2 transform.

    Vec2 a = Vec2::Transform(Vec2(50, 50),  //* Matrix3 transform.
        Mat3(0.707107F,
            0.707107F,
            0,
            -0.707107F,
            0.707107F,
            0,
            100.0F,
            -41.421356F,
            1.0F));  //* 45° rotation matrix with [100, 100] translation.

    EXPECT_EQ(a[0], 100.0F);
    EXPECT_EQ(a[1], 29.2893448F);
}

// Constructors

TEST(Vector2, ConstructorDefault) {
    Vector2<float> a;

    EXPECT_EQ(a[0], 0);
    EXPECT_EQ(a[1], 0);
}

TEST(Vector2, ConstructorBasedOnParameters) {
    Vec2 a(1, 2);

    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
}

TEST(Vector2, ConstructorBasedOnOtherVector2) {
    const Vec2 a;
    const Vec2 b;
    Vector2<Vec2> a3(a, b);
    auto a4 = a3;

    a3[0][0] = 1;

    EXPECT_EQ(a3[0][0], 1);
    EXPECT_NE(a3[0][0], a4[0][0]);
}

// Accessor operators

TEST(Vector2, AccessorOperatorAccessScalar) {
    Vec2 a(1, 5);

    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 5);
}

TEST(Vector2, AccessorOperatorConstAccessScalar) {  //~ Not calling the function.
    Vec2 a;

    const float x = a[(const int)0];
    const float y = a[(const int)1];

    EXPECT_EQ(a[0], x);
    EXPECT_EQ(a[1], y);
}

TEST(Vector2, AccessorOperatorCast) {
    Vector2<int> a(Vector2<float>(1.9F, 2.3F));

    EXPECT_EQ(a[0], (int)1);
    EXPECT_EQ(a[1], (int)2);
}

// Comparison Operators

TEST(Vector2, ComparisonOperatorEquals) {
    EXPECT_TRUE(Vec2(50, 50) == Vec2(50, 50));
    EXPECT_FALSE(Vec2(50, 50) == Vec2(5, 5));
}

TEST(Vector2, ComparisonOperatorDoesNotEqual) {
    EXPECT_TRUE(Vec2(5, 5) != Vec2(50, 50));
    EXPECT_FALSE(Vec2(5, 5) != Vec2(5, 5));
}

TEST(Vector2, ComparisonOperatorLessThan) {
    EXPECT_TRUE(Vec2(5, 5) < Vec2(50, 50));
    EXPECT_FALSE(Vec2(50, 50) < Vec2(5, 5));
}

TEST(Vector2, ComparisonOperatorLessThanOrEqual) {
    EXPECT_TRUE(Vec2(5, 5) <= Vec2(5, 5));
    EXPECT_FALSE(Vec2(50, 50) <= Vec2(5, 5));
}

TEST(Vector2, ComparisonOperatorGreaterThan) {
    EXPECT_TRUE(Vec2(50, 50) > Vec2(5, 5));
    EXPECT_FALSE(Vec2(5, 5) > Vec2(50, 50));
}

TEST(Vector2, ComparisonOperatorGreaterThanOrEqual) {
    EXPECT_TRUE(Vec2(50, 50) >= Vec2(50, 50));
    EXPECT_FALSE(Vec2(5, 5) >= Vec2(50, 50));
}

// Assignment Operators

TEST(Vector2, AssignmentOperatorDivideEquals) {
    EXPECT_EQ(Vec2(2.0F, 4.0F) /= 8.0F, Vec2(2.0F / 8.0F, 4.0F / 8.0F));
}

TEST(Vector2, AssignmentOperatorTimesEquals) {
    EXPECT_EQ(Vec2(2.0F, 4.0F) *= 8.0F, Vec2(16.0F, 32.0F));

    //* Matrix2 transform.

    //* Matrix3 transform.
}

TEST(Vector2, AssignmentOperatorPlusEquals) {
    EXPECT_EQ(Vec2(2.0F, 4.0F) += Vec2(1.0F, -1.0F), Vec2(3.0F, 3.0F));
}

TEST(Vector2, AssignmentOperatorMinusEquals) {
    EXPECT_EQ(Vec2(2.0F, 4.0F) -= Vec2(1.0F, -1.0F), Vec2(1.0F, 5.0F));
}

// Arithmetic Operators

TEST(Vector2, ArithmeticOperatorDivide) {
    EXPECT_EQ(Vec2(2.0F, 4.0F) / 8.0F, Vec2(2.0F / 8.0F, 4.0F / 8.0F));
}

TEST(Vector2, ArithmeticOperatorTimes) {
    EXPECT_EQ(Vec2(2.0F, 4.0F) * 8.0F, Vec2(16.0F, 32.0F));

    EXPECT_EQ(Vec2(1, 2) * Vec2(3, 4), 11);  //* Dot product.
    EXPECT_EQ(Vec2(10, 5) * Vec2(11, 7), 145);

    //* Matrix2 transform.

    //* Matrix3 transform.
}

TEST(Vector2, ArithmeticOperatorPlus) {
    EXPECT_EQ(Vec2(2.0F, 4.0F) + Vec2(1.0F, -1.0F), Vec2(3.0F, 3.0F));
}

TEST(Vector2, ArithmeticOperatorMinus) {
    EXPECT_EQ(Vec2(2.0F, 4.0F) - Vec2(1.0F, -1.0F), Vec2(1.0F, 5.0F));
}

TEST(Vector2, ArithmeticOperatorNegate) {
    EXPECT_EQ(-Vec2(2.0F, 4.0F), Vec2(-2.0F, -4.0F));
    EXPECT_NE(-Vec2(2.0F, 4.0F), -Vec2(-2.0F, -4.0F));
}

// Instance Methods

TEST(Vector2, InstanceMethodGetMagnitudeSquared) {
    EXPECT_EQ(Vec2(-3, -4).GetMagnitudeSquared(), 25);
}

TEST(Vector2, InstanceMethodGetMagnitude) {
    EXPECT_EQ(Vec2(0, 0).GetMagnitude(), 0);
    EXPECT_EQ(Vec2(3, 4).GetMagnitude(), 5);
    EXPECT_EQ(Vec2(0, 5).GetMagnitude(), 5);
}

TEST(Vector2, InstanceMethodGetNormalized) {
    EXPECT_EQ(Vec2(15, 15).GetNormalized(), Vec2(0.707106769F, 0.707106769F));
}

TEST(Vector2, InstanceMethodNormalize) {
    EXPECT_EQ(Vec2(10, 5).Normalize(), Vec2(0.89442718F, 0.44721359F));
}

TEST(Vector2, InstanceMethodNegate) {
    EXPECT_EQ(Vec2(1, 15).Negate(), Vec2(-1, -15));
}

TEST(Vector2, InstanceMethoRotate) {
    Vec2 a(1, -1);

    a.Rotate(Math::PI<float> / 2);
    EXPECT_NEAR(a[0], 1.0F, 1e-6);
    EXPECT_NEAR(a[1], 1.0F, 1e-6);

    a.Rotate(-Math::PI<float>);
    EXPECT_NEAR(a[0], -1.0F, 1e-6);
    EXPECT_NEAR(a[1], -1.0F, 1e-6);

    Vec2 b(1, -1);

    b.Rotate(Vec2(1, 0), Math::PI<float> / 2);  //* Rotate around point.
    EXPECT_NEAR(b[0], 2.0F, 1e-6);
    EXPECT_NEAR(b[1], 0.0F, 1e-6);
}

TEST(Vector2, InstanceMethodLerp) {
    EXPECT_EQ(Vec2(2, 1).Lerp(Vec2(3, -2), 0.73F), Vec2(2.73F, -1.19F));
}

TEST(Vector2, InstanceMethodPrint) {
}
