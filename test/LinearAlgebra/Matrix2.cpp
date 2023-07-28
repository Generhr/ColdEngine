#include "LinearAlgebra/Vector3.h"

#include <gtest/gtest.h>

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
    const Vec3 test1(0, 0, 0);
    const Vec3 test2(10, 10, 10);

    for (int i = 0; i <= 10; i++) {
        EXPECT_EQ(Vec3::Lerp(test1, test2, i / 10.0F), Vec3(i, i, i));
    }
}

TEST(Vector3, StaticFunctionTransform) {
    Vec3 test = Vec3::Transform(Vec3(2, 1, -3), Mat3(1.29F, 1, 0, 0, 0.73F, 0, 0, 0, 2.34F));  //* Matrix3 transform.

    EXPECT_NEAR(test[0], 2.58F, 1e-6);
    EXPECT_NEAR(test[1], 2.73F, 1e-6);
    EXPECT_NEAR(test[2], -7.02F, 1e-6);
}

// Constructors

TEST(Vector3, ConstructorDefault) {
    Vector3<float> test;

    EXPECT_EQ(test[0], 0);
    EXPECT_EQ(test[1], 0);
    EXPECT_EQ(test[2], 0);
}

TEST(Vector3, ConstructorBasedOnParameters) {
    Vec3 test(1, 9, 5);

    EXPECT_EQ(test[0], 1);
    EXPECT_EQ(test[1], 9);
    EXPECT_EQ(test[2], 5);
}

TEST(Vector3, ConstructorBasedOnOtherVector3) {
    const Vec3 test1;
    const Vec3 test2;
    const Vec3 test3;
    Vector3<Vec3> test4(test1, test2, test3);
    auto test5 = test4;

    test4[0][0] = 1;

    EXPECT_NE(test4[0][0], test5[0][0]);
}

// Accessor operators

TEST(Vector3, AccessorOperatorAccessScalar) {
    Vec3 test(1, 5, 2);

    EXPECT_EQ(test[0], 1);
    EXPECT_EQ(test[1], 5);
    EXPECT_EQ(test[2], 2);
}

TEST(Vector3, AccessorOperatorConstAccessScalar) {  //~ Not calling the function.
    Vec3 test(1, 8, 2);

    const float x = test[(const int)0];
    const float y = test[(const int)1];
    const float z = test[(const int)2];

    EXPECT_EQ(test[0], x);
    EXPECT_EQ(test[1], y);
    EXPECT_EQ(test[2], z);
}

TEST(Vector3, AccessorOperatorCast) {
    Vector3<int> test(Vector3<float>(1.9F, 2.3F, 5.3F));

    EXPECT_EQ(test[0], (int)1);
    EXPECT_EQ(test[1], (int)2);
    EXPECT_EQ(test[2], (int)5);
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
    EXPECT_EQ(Vec3(2.0F, 4.0F, 8.0F) /= 8.0F, Vec3(2.0F / 8.0F, 4.0F / 8.0F, 1.0F));
}

TEST(Vector3, AssignmentOperatorTimesEquals) {
    EXPECT_EQ(Vec3(2.0F, 4.0F, 8.0F) *= 8.0F, Vec3(16.0F, 32.0F, 64.0F));

    //* Matrix3 transform.
}

TEST(Vector3, AssignmentOperatorPlusEquals) {
    EXPECT_EQ(Vec3(2.0F, 4.0F, 8.0F) += Vec3(1.0F, -1.0F, 0.0F), Vec3(3.0F, 3.0F, 8.0F));
}

TEST(Vector3, AssignmentOperatorMinusEquals) {
    EXPECT_EQ(Vec3(2.0F, 4.0F, 8.0F) -= Vec3(1.0F, -1.0F, 0.0F), Vec3(1.0F, 5.0F, 8.0F));
}

// Arithmetic Operators

TEST(Vector3, ArithmeticOperatorDivide) {
    EXPECT_EQ(Vec3(2.0F, 4.0F, 8.0F) / 8.0F, Vec3(2.0F / 8.0F, 4.0F / 8.0F, 1.0F));
}

TEST(Vector3, ArithmeticOperatorTimes) {
    EXPECT_EQ(Vec3(2.0F, 4.0F, 8.0F) * 8.0F, Vec3(16.0F, 32.0F, 64.0F));

    EXPECT_EQ(Vec3(0, 5, 2) * Vec3(3, 0, 5), 10);  //* Dot product.
    EXPECT_EQ(Vec3(-5, -5, 5) * Vec3(0, 7, -2), -45);

    //* Matrix3 transform.
}

TEST(Vector3, ArithmeticOperatorPlus) {
    EXPECT_EQ(Vec3(2.0F, 4.0F, 8.0F) + Vec3(1.0F, -1.0F, 0.0F), Vec3(3.0F, 3.0F, 8.0F));
}

TEST(Vector3, ArithmeticOperatorMinus) {
    EXPECT_EQ(Vec3(2.0F, 4.0F, 8.0F) - Vec3(1.0F, -1.0F, 0.0F), Vec3(1.0F, 5.0F, 8.0F));
}

TEST(Vector3, ArithmeticOperatorNegate) {
    EXPECT_EQ(-Vec3(2.0F, 4.0F, -8.0F), Vec3(-2.0F, -4.0F, 8.0F));
    EXPECT_NE(-Vec3(2.0F, 4.0F, -8.0F), -Vec3(-2.0F, -4.0F, 8.0F));
}

// Instance Methods

TEST(Vector3, InstanceMethodGetMagnitudeSquared) {
    EXPECT_EQ(Vec3(-3, -4, -5).GetMagnitudeSquared(), 50);
}

TEST(Vector3, InstanceMethodGetMagnitude) {
    EXPECT_EQ(Vec3(0, 10, 0).GetMagnitude(), 10);
    EXPECT_NEAR(Vec3(3, 4, 5).GetMagnitude(), sqrt(50), 1e-6);
    EXPECT_NEAR(Vec3(-4, 3, 8).GetMagnitude(), sqrt(89), 1e-6);
}

TEST(Vector3, InstanceMethodGetNormalized) {
    Vec3 test1(5, 5, 5);
    test1 = test1.GetNormalized();

    EXPECT_NEAR(test1[0], 0.577350F, 1e-6);
    EXPECT_NEAR(test1[1], 0.577350F, 1e-6);
    EXPECT_NEAR(test1[2], 0.577350F, 1e-6);
}

TEST(Vector3, InstanceMethodNormalize) {
    Vec3 test1(3, 1, 2);
    test1.Normalize();

    EXPECT_NEAR(test1[0], 0.801784F, 1e-6);
    EXPECT_NEAR(test1[1], 0.267261F, 1e-6);
    EXPECT_NEAR(test1[2], 0.534522F, 1e-6);

    // EXPECT_FLOAT_EQ()
}

TEST(Vector3, InstanceMethodNegate) {
    EXPECT_EQ(Vec3(1, 15, 5).Negate(), Vec3(-1, -15, -5));
}

TEST(Vector3, InstanceMethodLerp) {
    EXPECT_EQ(Vec3(1, 2, 0).Lerp(Vec3(2, 1, 1), 0.25F), Vec3(1.25F, 1.75F, 0.25F));
}

TEST(Vector3, InstanceMethodPrint) {
}
