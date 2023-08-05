#pragma once

#include "Scene.h"
#include "CubeFolded.h"
#include "PubeScreenTransformer.h"
#include "LinearAlgebra/Matrix3.h"

class FoldedCubeScene : public Scene {
public:
    FoldedCubeScene() : Scene("Folded Textured Clamped Cube") {
    }

    virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override {
        if (kbd.KeyIsPressed('Q')) {
            theta_x = Math::wrapAngle(theta_x + dTheta * dt);
        }

        if (kbd.KeyIsPressed('W')) {
            theta_y = Math::wrapAngle(theta_y + dTheta * dt);
        }

        if (kbd.KeyIsPressed('E')) {
            theta_z = Math::wrapAngle(theta_z + dTheta * dt);
        }

        if (kbd.KeyIsPressed('A')) {
            theta_x = Math::wrapAngle(theta_x - dTheta * dt);
        }

        if (kbd.KeyIsPressed('S')) {
            theta_y = Math::wrapAngle(theta_y - dTheta * dt);
        }

        if (kbd.KeyIsPressed('D')) {
            theta_z = Math::wrapAngle(theta_z - dTheta * dt);
        }

        if (kbd.KeyIsPressed('R')) {
            offset_z += 2.0f * dt;
        }

        if (kbd.KeyIsPressed('F')) {
            offset_z -= 2.0f * dt;
        }
    }

    virtual void Draw(Graphics& gfx) const override {
        // NOTE: Changes had to be made because of Vec3 -> TexVertex
        // Generate indexed triangle list
        auto triangles = cube.GetTrianglesTex();
        // Generate rotation matrix from euler angles
        const Mat3 rot = Mat3::CreateXRotationMatrix(theta_x) * Mat3::CreateYRotationMatrix(theta_y) *
                         Mat3::CreateZRotationMatrix(theta_z);

        // Transform from model space -> world (/view) space
        for (auto& v : triangles.vertices) {
            v.pos *= rot;
            v.pos += {0.0f, 0.0f, offset_z};
        }

        // Backface culling test (must be done in world (/view) space)
        for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++) {
            const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]].pos;
            const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]].pos;
            const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]].pos;

            triangles.cullFlags[i] = Vec3::CrossProduct((v1 - v0), (v2 - v0)) * v0 > 0.0f;
        }

        // Transform to screen space (includes perspective transform)
        for (auto& v : triangles.vertices) {
            pst.Transform(v.pos);
        }

        // Draw the mf triangles!
        for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++) {
            // Skip triangles previously determined to be back-facing
            if (!triangles.cullFlags[i]) {
                gfx.DrawTriangleTex(triangles.vertices[triangles.indices[i * 3]],
                    triangles.vertices[triangles.indices[i * 3 + 1]],
                    triangles.vertices[triangles.indices[i * 3 + 2]],
                    sbTex);
            }
        }
    }

private:
    PubeScreenTransformer pst;
    CubeFolded cube = CubeFolded(1.0f);
    Surface sbTex = Surface::FromFile(L"res\\sauron-bhole-100x100.png");
    static constexpr float dTheta = Math::PI<float>;
    float offset_z = 2.0f;
    float theta_x = 0.0f;
    float theta_y = 0.0f;
    float theta_z = 0.0f;
};
