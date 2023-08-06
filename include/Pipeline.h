#pragma once

#include "Win.h"
#include "Graphics.h"
#include "Triangle.h"
#include "IndexedTriangleList.h"
#include "PubeScreenTransformer.h"
#include "LinearAlgebra/Matrix3.h"
#include "ZBuffer.h"

#include <algorithm>


// Triangle drawing pipeline with programable pixel shading stage
template<class Effect>
class Pipeline {
public:
    typedef typename Effect::Vertex Vertex;

public:
    explicit Pipeline(Graphics& graphics) : graphics(graphics), zb(graphics.ScreenWidth, graphics.ScreenHeight) {
    }

    void Draw(const IndexedTriangleList<Vertex>& triList) {
        ProcessVertices(triList.vertices, triList.indices);
    }

    void BindRotation(const Mat3& rotation_in) {
        rotation = rotation_in;
    }

    void BindTranslation(const Vec3& translation_in) {
        translation = translation_in;
    }

    // Needed to reset the z-buffer after each frame
    void BeginFrame() {
        zb.Clear();
    }

private:
    // Vertex Processing Function

    // Transforms vertices and then passes vtx & idx lists to triangle assembler
    void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices) {
        // Create vertex vector for vs output
        std::vector<Vertex> verticesOut;

        // Transform vertices using matrix + vector
        std::transform(vertices.begin(), vertices.end(), std::back_inserter(verticesOut), [&](const auto& v) {
            return Vertex(v.pos * rotation + translation, v);
        });

        // Assemble triangles from stream of indices and vertices
        AssembleTriangles(verticesOut, indices);
    }

    // Triangle Assembly Function

    // Assembles indexed vertex stream into triangles and passes them to post process culls (does not send) back facing
    // triangles
    void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices) {
        // Assemble triangles in the stream and process
        for (size_t i = 0, end = indices.size() / 3; i < end; i++) {
            // Determine triangle vertices via indexing
            const auto& v0 = vertices[indices[i * 3]];
            const auto& v1 = vertices[indices[i * 3 + 1]];
            const auto& v2 = vertices[indices[i * 3 + 2]];

            // Cull backfacing triangles with cross product
            if (Vec3::CrossProduct((v1.pos - v0.pos), (v2.pos - v0.pos)) * v0.pos <= 0.0f) {
                // Process 3 vertices into a triangle
                ProcessTriangle(v0, v1, v2);
            }
        }
    }

    // Triangle Processing Function

    // Takes 3 vertices to generate triangle and sends generated triangle to post-processing
    void ProcessTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
        // Generate triangle from 3 vertices using gs and send to post-processing
        Triangle<Vertex> a {v0, v1, v2};

        PostProcessTriangleVertices(a);
    }

    // Vertex Post-processing Function

    // Perform perspective and viewport transformations
    void PostProcessTriangleVertices(Triangle<Vertex>& triangle) {
        // Perspective divide and screen transform for all 3 vertices
        pst.Transform(triangle.v0);
        pst.Transform(triangle.v1);
        pst.Transform(triangle.v2);

        // Draw the triangle
        DrawTriangle(triangle);
    }

    // Triangle Rasterization Functions

    //    it0, it1, etc. stand for interpolants
    //    (values which are interpolated across a triangle in screen space)
    //
    // Entry point for triangle rasterization, sorts vertices, determines case, splits to flat triangles, dispatches to
    // flat triangle functions
    void DrawTriangle(const Triangle<Vertex>& triangle) {
        // Using pointers so we can swap (for sorting purposes)
        const Vertex* pv0 = &triangle.v0;
        const Vertex* pv1 = &triangle.v1;
        const Vertex* pv2 = &triangle.v2;

        // Sorting vertices by y
        if (pv1->pos[1] < pv0->pos[1]) {
            std::swap(pv0, pv1);
        }

        if (pv2->pos[1] < pv1->pos[1]) {
            std::swap(pv1, pv2);
        }

        if (pv1->pos[1] < pv0->pos[1]) {
            std::swap(pv0, pv1);
        }

        if (pv0->pos[1] == pv1->pos[1]) {  // Natural flat top
            // Sorting top vertices by x
            if (pv1->pos[0] < pv0->pos[0]) {
                std::swap(pv0, pv1);
            }

            DrawFlatTopTriangle(*pv0, *pv1, *pv2);
        }
        else if (pv1->pos[1] == pv2->pos[1]) {  // Natural flat bottom
            // Sorting bottom vertices by x
            if (pv2->pos[0] < pv1->pos[0]) {
                std::swap(pv1, pv2);
            }

            DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
        }
        else {  // General triangle
            // Find splitting vertex interpolant
            const float alphaSplit = (pv1->pos[1] - pv0->pos[1]) / (pv2->pos[1] - pv0->pos[1]);
            const auto vi = Math::Lerp(*pv0, *pv2, alphaSplit);

            if (pv1->pos[0] < vi.pos[0]) {  // Major right
                DrawFlatBottomTriangle(*pv0, *pv1, vi);
                DrawFlatTopTriangle(*pv1, vi, *pv2);
            }
            else {  // Major left
                DrawFlatBottomTriangle(*pv0, vi, *pv1);
                DrawFlatTopTriangle(vi, *pv1, *pv2);
            }
        }
    }

    // Does flat *TOP* triangle-specific calculations and calls DrawFlatTriangle
    void DrawFlatTopTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2) {
        // Calulcate dVertex / dy change in interpolant for every 1 change in y
        const float delta_y = it2.pos[1] - it0.pos[1];
        const auto dit0 = (it2 - it0) / delta_y;
        const auto dit1 = (it2 - it1) / delta_y;

        // Create right edge interpolant
        auto itEdge1 = it1;

        // Call the flat triangle render routine
        DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
    }

    // Does flat *BOTTOM* triangle-specific calculations and calls DrawFlatTriangle
    void DrawFlatBottomTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2) {
        // Calulcate dVertex / dy change in interpolant for every 1 change in y
        const float delta_y = it2.pos[1] - it0.pos[1];
        const auto dit0 = (it1 - it0) / delta_y;
        const auto dit1 = (it2 - it0) / delta_y;

        // Create right edge interpolant
        auto itEdge1 = it0;

        // Call the flat triangle render routine
        DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
    }

    // Does processing common to both flat top and flat bottom triangles scan over triangle in screen space, interpolate
    // attributes, depth cull, invoke ps and write pixel to screen
    void DrawFlatTriangle(const Vertex& it0,
        const Vertex& it1,
        const Vertex& it2,
        const Vertex& dv0,
        const Vertex& dv1,
        Vertex itEdge1) {
        // Create edge interpolant for left edge (always v0)
        auto itEdge0 = it0;

        // Calculate start and end scanlines
        const int yStart = static_cast<int>(ceil(it0.pos[1] - 0.5f));
        const int yEnd = static_cast<int>(ceil(it2.pos[1] - 0.5f));  // The scanline AFTER the last line drawn

        // Do interpolant prestep
        itEdge0 += dv0 * (static_cast<float>(yStart) + 0.5f - it0.pos[1]);
        itEdge1 += dv1 * (static_cast<float>(yStart) + 0.5f - it0.pos[1]);

        for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1) {
            // Calculate start and end pixels
            const int xStart = static_cast<int>(ceil(itEdge0.pos[0] - 0.5f));
            const int xEnd = static_cast<int>(ceil(itEdge1.pos[0] - 0.5f));  // The pixel AFTER the last pixel drawn

            // Create scanline interpolant startpoint (some waste for interpolating x,y,z, but makes life easier not
            // having to split them off, and z will be needed in the future anyways...)
            auto iLine = itEdge0;

            // Calculate delta scanline interpolant / dx
            const float dx = itEdge1.pos[0] - itEdge0.pos[0];
            const auto diLine = (itEdge1 - iLine) / dx;

            // Prestep scanline interpolant
            iLine += diLine * (static_cast<float>(xStart) + 0.5f - itEdge0.pos[0]);

            for (int x = xStart; x < xEnd;
                 x++, iLine += diLine) {  //~ Rasterization: a Practical Implementation: https://tinyurl.com/7m9ssz7f
                                          //~ Perspective-Correct Interpolation: https://tinyurl.com/mrrzxc7c
                // Recover interpolated `z` from interpolated `1 / z`
                const float z = 1.0f / iLine.pos[2];

                // Do z rejection / update of z buffer
                // skip shading step if z rejected (early z)
                if (zb.TestAndSet(x, y, z)) {  //~ Depth Precision Visualized: https://tinyurl.com/2fmpcmzw
                    // Recover interpolated attributes (wasted effort in multiplying `pos` (x, y, z) here, but not a
                    // huge deal, not worth the code complication to fix)
                    const auto attr = iLine * z;

                    // Invoke pixel shader with interpolated vertex attributes and use the result to set the pixel color
                    // on the screen
                    graphics.PutPixel(x, y, effect.ps(attr));
                }
            }
        }
    }

public:
    Effect effect;

private:
    Graphics& graphics;
    ZBuffer zb;
    PubeScreenTransformer pst;
    Mat3 rotation;
    Vec3 translation;
};
