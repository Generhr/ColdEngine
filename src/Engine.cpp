#include "Engine.h"

#include "MainWindow.h"
#include "Scenes/CubeSkinScene.h"
#include "Scenes/CubeSolidScene.h"
#include "Scenes/CubeVertexColorScene.h"
#include "Scenes/DoubleCubeScene.h"
#include "Scenes/CubeVertexPositionColorScene.h"
#include "Scenes/CubeSolidGeometryScene.h"
#include "Scenes/CubeFlatIndependentScene.h"
#include "Scenes/GeometryFlatScene.h"
#include "Scenes/VertexWaveScene.h"

Engine::Engine(MainWindow& window) : window(window), graphics(window) {
    // scenes.push_back(std::make_unique<CubeSkinScene>(graphics, L"res\\office_skin.jpg"));
    // scenes.push_back(std::make_unique<CubeSolidScene>(graphics));
    // scenes.push_back(std::make_unique<CubeVertexColorScene>(graphics));
    // scenes.push_back(std::make_unique<DoubleCubeScene>(graphics));
    // scenes.push_back(std::make_unique<CubeVertexPositionColorScene>(graphics));
    scenes.push_back(std::make_unique<CubeSolidGeometryScene>(graphics));
    scenes.push_back(std::make_unique<CubeFlatIndependentScene>(graphics));
    scenes.push_back(std::make_unique<GeometryFlatScene>(graphics, Cube::GetPlain<GeometryFlatScene::Vertex>()));
    scenes.push_back(std::make_unique<GeometryFlatScene>(graphics,
        IndexedTriangleList<GeometryFlatScene::Vertex>::Load("res\\bunny.obj")));
    scenes.push_back(std::make_unique<VertexWaveScene>(graphics));


    curScene = scenes.begin();
    SetWindowName();
}

void Engine::UpdateModel() {
    const float dt = ft.Mark();

    // Cycle through scenes when tab is pressed
    while (!window.kbd.KeyIsEmpty()) {
        const auto e = window.kbd.ReadKey();

        if (e.GetCode() == VK_TAB && e.IsPress()) {
            if (window.kbd.KeyIsPressed(VK_SHIFT)) {
                ReverseCycleScenes();
            }
            else {
                CycleScenes();
            }
        }
        else if (e.GetCode() == VK_ESCAPE && e.IsPress()) {
            window.Kill();
        }
    }

    // Update scene
    (*curScene)->Update(window.kbd, window.mouse, dt);
}

void Engine::ComposeFrame() {
    graphics.BeginFrame();

    // Draw scene
    (*curScene)->Draw();

    graphics.EndFrame();
}

void Engine::CycleScenes() {
    if (++curScene == scenes.end()) {
        curScene = scenes.begin();
    }

    SetWindowName();
}

void Engine::ReverseCycleScenes() {
    if (curScene == scenes.begin()) {
        curScene = scenes.end() - 1;
    }
    else {
        --curScene;
    }

    SetWindowName();
}

void Engine::SetWindowName() const {
    SetWindowTextW(window.GetHWND(),
        (*curScene)->GetName().c_str());  //~ Changed `GetName()` to wide to avoid "potential loss of data" warnings
}
