#pragma once

#include <vector>

#include "LinearAlgebra/Vector3.h"
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"

class XQuads {
public:
    explicit XQuads(float size) {
        const float side = size / 2.0f;
        vertices.emplace_back(-side, -side, -side);
        vertices.emplace_back(side, -side, -side);
        vertices.emplace_back(-side, side, -side);
        vertices.emplace_back(side, side, -side);
        vertices.emplace_back(-side, -side, side);
        vertices.emplace_back(side, -side, side);
        vertices.emplace_back(-side, side, side);
        vertices.emplace_back(side, side, side);
    }

    IndexedLineList GetLines() const {
        return {vertices, {0, 4, 4, 7, 7, 3, 3, 0, 1, 5, 5, 6, 6, 2, 2, 1}};
    }

    IndexedTriangleList<Vec3> GetTriangles() const {
        return {vertices, {0, 4, 3, 4, 7, 3, 2, 6, 1, 6, 5, 1}};
    }

private:
    std::vector<Vec3> vertices;
};
