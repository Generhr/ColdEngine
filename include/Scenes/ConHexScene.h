#pragma once

#include "Scene.h"
#include "ConcaveHexahedron.h"
#include "PubeScreenTransformer.h"
#include "LinearAlgebra/Matrix3.h"

class ConHexScene : public Scene {
public:
    ConHexScene() : Scene("Concave Hexahedron Nasty Draw Order") {
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

    virtual void Draw(Graphics& graphics) const override {
        // Generate indexed triangle list
        auto triangles = hex.GetTriangles();
        // Generate rotation matrix from euler angles
        const Mat3 rot = Mat3::CreateXRotationMatrix(theta_x) * Mat3::CreateYRotationMatrix(theta_y) *
                         Mat3::CreateZRotationMatrix(theta_z);

        // Transform from model space -> world (/view) space
        for (auto& v : triangles.vertices) {
            v *= rot;
            v += {0.0f, 0.0f, offset_z};
        }

        // Backface culling test (must be done in world (/view) space)
        for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++) {
            const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
            const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
            const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];

            triangles.cullFlags[i] = Vec3::CrossProduct((v1 - v0), (v2 - v0)) * v0 > 0.0f;
        }

        // Transform to screen space (includes perspective transform)
        for (auto& v : triangles.vertices) {
            pst.Transform(v);
        }

        // Draw the mf triangles!
        for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++) {
            // Skip triangles previously determined to be back-facing
            if (!triangles.cullFlags[i]) {
                graphics.DrawTriangle(triangles.vertices[triangles.indices[i * 3]],
                    triangles.vertices[triangles.indices[i * 3 + 1]],
                    triangles.vertices[triangles.indices[i * 3 + 2]],
                    colors[i]);
            }
        }
    }

private:
    PubeScreenTransformer pst;
    ConcaveHexahedron hex = ConcaveHexahedron(1.0f);
    static constexpr Color colors[12] = {
        Colors::White,
        Colors::Blue,
        Colors::Cyan,
        Colors::Yellow,
        Colors::Green,
        Colors::Magenta,
    };
    static constexpr float dTheta = Math::PI<float>;
    float offset_z = 2.0f;
    float theta_x = 0.0f;
    float theta_y = 0.0f;
    float theta_z = 0.0f;
};
