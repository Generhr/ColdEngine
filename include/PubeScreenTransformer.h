#pragma once

#include "Graphics.h"
#include "LinearAlgebra/Vector3.h"

class PubeScreenTransformer {
public:
    PubeScreenTransformer()
        : xFactor(static_cast<float>(Graphics::ScreenWidth) / 2.0f),
          yFactor(static_cast<float>(Graphics::ScreenHeight) / 2.0f) {
    }

    Vec3& Transform(Vec3& v) const {
        const float zInv = 1.0f / v[2];
        v[0] = (v[0] * zInv + 1.0f) * xFactor;
        v[1] = (-v[1] * zInv + 1.0f) * yFactor;
        return v;
    }

    //  Vec3 GetTransformed(const Vec3& v) const {
    //      return Transform(Vec3(v));
    //  }

private:
    float xFactor;
    float yFactor;
};
