#pragma once

#include "Graphics.h"
#include "LinearAlgebra/Vector3.h"


class PubeScreenTransformer {
public:
    PubeScreenTransformer()
        : xFactor(static_cast<float>(Graphics::ScreenWidth) / 2.0f),
          yFactor(static_cast<float>(Graphics::ScreenHeight) / 2.0f) {
    }

    template<class Vertex>
    Vertex& Transform(Vertex& v) const {
        const float zInv = 1.0f / v.pos[2];

        // Divide all position components and attributes by `z` (we want to be interpolating our attributes in the same
        // space where the `x, y` interpolation is taking place to prevent distortion)
        v *= zInv;
        // Adjust position `x, y` from perspective normalized space to screen dimension space after perspective divide
        v.pos[0] = (v.pos[0] + 1.0f) * xFactor;
        v.pos[1] = (-v.pos[1] + 1.0f) * yFactor;
        // Store `1 / z` in `z` (we will need the interpolated `1 / z` so that we can recover the attributes after
        // interperlation.)
        v.pos[2] = zInv;

        return v;
    }

    template<class Vertex>
    Vertex GetTransformed(const Vertex& v) const {
        return Transform(Vertex(v));
    }

private:
    float xFactor;
    float yFactor;
};
