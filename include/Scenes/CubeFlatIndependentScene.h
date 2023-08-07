#pragma once

#include "Scene.h"
#include "Primitives/Cube.h"
#include "LinearAlgebra/Matrix3.h"
#include "Pipeline.h"
#include "Effects/VertexFlatEffect.h"


class CubeFlatIndependentScene : public Scene {
public:
    typedef Pipeline<VertexFlatEffect> Pipeline;
    typedef Pipeline::Vertex Vertex;

public:
    explicit CubeFlatIndependentScene(Graphics& graphics)
        : itlist(Cube::GetIndependentFacesNormals<Vertex>()), pipeline(graphics), Scene(L"Cube flat vertex scene") {
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

        if (kbd.KeyIsPressed('U')) {
            phi_x = Math::WrapAngle(phi_x + dTheta * dt);
        }

        if (kbd.KeyIsPressed('I')) {
            phi_y = Math::WrapAngle(phi_y + dTheta * dt);
        }

        if (kbd.KeyIsPressed('O')) {
            phi_z = Math::WrapAngle(phi_z + dTheta * dt);
        }

        if (kbd.KeyIsPressed('J')) {
            phi_x = Math::WrapAngle(phi_x - dTheta * dt);
        }

        if (kbd.KeyIsPressed('K')) {
            phi_y = Math::WrapAngle(phi_y - dTheta * dt);
        }

        if (kbd.KeyIsPressed('L')) {
            phi_z = Math::WrapAngle(phi_z - dTheta * dt);
        }

        if (kbd.KeyIsPressed('R')) {
            offset_z += 0.2f * dt;
        }

        if (kbd.KeyIsPressed('F')) {
            offset_z -= 0.2f * dt;
        }
    }

    virtual void Draw() override {
        pipeline.BeginFrame();

        // Generate rotation matrix from euler angles
        const Mat3 rot = Mat3::CreateXRotationMatrix(theta_x) * Mat3::CreateYRotationMatrix(theta_y) *
                         Mat3::CreateZRotationMatrix(theta_z);
        const Mat3 rot_phi = Mat3::CreateXRotationMatrix(phi_x) * Mat3::CreateYRotationMatrix(phi_y) *
                             Mat3::CreateZRotationMatrix(phi_z);
        // Translation from offset
        const Vec3 trans = {0.0f, 0.0f, offset_z};

        // Set pipeline transform
        pipeline.effect.vs.BindRotation(rot);
        pipeline.effect.vs.BindTranslation(trans);
        pipeline.effect.vs.SetLightDirection(light_dir * rot_phi);
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
    float phi_x = 0.0f;
    float phi_y = 0.0f;
    float phi_z = 0.0f;
    Vec3 light_dir = {0.2f, -0.5f, 1.0f};
};
