#include "Scene.h"

#include "LinearAlgebra/Matrix3.h"
#include "MainWindow.h"

Scene::Scene(MainWindow& window) : window(window), graphics(window), cube(1.0f) {
}

void Scene::UpdateModel() {
    const float dt = 1.0f / 60.0f;

    if (window.kbd.KeyIsPressed('Q')) {
        theta_x = Math::wrapAngle(theta_x + dTheta * dt);
    }

    if (window.kbd.KeyIsPressed('W')) {
        theta_y = Math::wrapAngle(theta_y + dTheta * dt);
    }

    if (window.kbd.KeyIsPressed('E')) {
        theta_z = Math::wrapAngle(theta_z + dTheta * dt);
    }

    if (window.kbd.KeyIsPressed('A')) {
        theta_x = Math::wrapAngle(theta_x - dTheta * dt);
    }

    if (window.kbd.KeyIsPressed('S')) {
        theta_y = Math::wrapAngle(theta_y - dTheta * dt);
    }

    if (window.kbd.KeyIsPressed('D')) {
        theta_z = Math::wrapAngle(theta_z - dTheta * dt);
    }

    if (window.kbd.KeyIsPressed('R')) {
        offset_z += 2.0f * dt;
    }

    if (window.kbd.KeyIsPressed('F')) {
        offset_z -= 2.0f * dt;
    }
}

void Scene::ComposeFrame() {
    graphics.BeginFrame();

    auto lines = cube.GetLines();
    const Mat3 rot = Mat3::CreateXRotationMatrix(theta_x) * Mat3::CreateYRotationMatrix(theta_y) *
                     Mat3::CreateZRotationMatrix(theta_z);

    for (auto& v : lines.vertices) {
        v *= rot;
        v += {0.0f, 0.0f, offset_z};
        pst.Transform(v);
    }

    for (auto i = lines.indices.cbegin(), end = lines.indices.cend(); i != end; std::advance(i, 2)) {
        graphics.DrawLine(lines.vertices[*i], lines.vertices[*std::next(i)], Colors::White);
    }

    graphics.EndFrame();
}
