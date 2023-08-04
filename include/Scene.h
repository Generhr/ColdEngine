#pragma once

#include "Cube.h"
#include "Graphics.h"
#include "PubeScreenTransformer.h"

class Scene {
public:
    explicit Scene(class MainWindow& window);

    ~Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    void UpdateModel();
    void ComposeFrame();

private:
    MainWindow& window;
    Graphics graphics;

    /********************************/
    /*  User Variables              */
    PubeScreenTransformer pst;
    Cube cube;
    const float dTheta = Math::PI<float>;
    float offset_z = 2.0f;
    float theta_x = 0.0f;
    float theta_y = 0.0f;
    float theta_z = 0.0f;
    /********************************/
};
