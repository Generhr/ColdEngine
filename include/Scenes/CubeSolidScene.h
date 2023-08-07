#pragma once

#include "Scene.h"
#include "Primitives/Cube.h"
#include "LinearAlgebra/Matrix3.h"
#include "Pipeline.h"
#include "Effects/SolidEffect.h"


class CubeSolidScene : public Scene {
public:
    typedef Pipeline<SolidEffect> Pipeline;
    typedef Pipeline::Vertex Vertex;

public:
    explicit CubeSolidScene(Graphics& graphics)
        : itlist(Cube::GetPlainIndependentFaces<Vertex>()), pipeline(graphics),
          Scene(L"Colored cube solid independent faces scene") {
        const Color colors[] =
            {Colors::Red, Colors::Green, Colors::Blue, Colors::Magenta, Colors::Yellow, Colors::Cyan};

        for (int i = 0; i < itlist.vertices.size(); i++) {
            itlist.vertices[i].color = colors[i / 4];
        }
    }

    virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override {
        if (kbd.KeyIsPressed('Q')) {
            theta_x = Math::WrapAngle(theta_x + dTheta * dt);
        }

        if (kbd.KeyIsPressed('W')) {
            theta_y = Math::WrapAngle(theta_y + dTheta * dt);
        }

        if (kbd.KeyIsPressed('E')) {
            theta_z = Math::WrapAngle(theta_z + dTheta * dt);
        }

        if (kbd.KeyIsPressed('A')) {
            theta_x = Math::WrapAngle(theta_x - dTheta * dt);
        }

        if (kbd.KeyIsPressed('S')) {
            theta_y = Math::WrapAngle(theta_y - dTheta * dt);
        }

        if (kbd.KeyIsPressed('D')) {
            theta_z = Math::WrapAngle(theta_z - dTheta * dt);
        }

        if (kbd.KeyIsPressed('R')) {
            offset_z += 2.0f * dt;
        }

        if (kbd.KeyIsPressed('F')) {
            offset_z -= 2.0f * dt;
        }
    }

    virtual void Draw() override {
        pipeline.BeginFrame();

        // Generate rotation matrix from euler angles
        const Mat3 rot = Mat3::CreateXRotationMatrix(theta_x) * Mat3::CreateYRotationMatrix(theta_y) *
                         Mat3::CreateZRotationMatrix(theta_z);
        // Translation from offset
        const Vec3 trans = {0.0f, 0.0f, offset_z};

        // Set pipeline transform
        pipeline.effect.vs.BindRotation(rot);
        pipeline.effect.vs.BindTranslation(trans);
        // Render triangles
        pipeline.Draw(itlist);
    }

private:
    IndexedTriangleList<Vertex> itlist;
    Pipeline pipeline;

    static constexpr float dTheta = Math::PI<float>;
    float offset_z = 2.0f;
    float theta_x = 0.0f;
    float theta_y = 0.0f;
    float theta_z = 0.0f;
};
