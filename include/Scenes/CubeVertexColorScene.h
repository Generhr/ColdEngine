#pragma once

#include "Scene.h"
#include "Cube.h"
#include "LinearAlgebra/Matrix3.h"
#include "Pipeline.h"
#include "Effect/VertexColorEffect.h"


class CubeVertexColorScene : public Scene {
public:
    typedef Pipeline<VertexColorEffect> Pipeline;
    typedef Pipeline::Vertex Vertex;

public:
    explicit CubeVertexColorScene(Graphics& graphics)
        : itlist(Cube::GetPlain<Vertex>()), pipeline(graphics), Scene(L"Colored cube vertex gradient scene") {
        itlist.vertices[0].color = Vec3(Colors::Red);
        itlist.vertices[1].color = Vec3(Colors::Green);
        itlist.vertices[2].color = Vec3(Colors::Blue);
        itlist.vertices[3].color = Vec3(Colors::Yellow);
        itlist.vertices[4].color = Vec3(Colors::Cyan);
        itlist.vertices[5].color = Vec3(Colors::Magenta);
        itlist.vertices[6].color = Vec3(Colors::White);
        itlist.vertices[7].color = Vec3(Colors::Black);
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
        pipeline.BindRotation(rot);
        pipeline.BindTranslation(trans);
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
