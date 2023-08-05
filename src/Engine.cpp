#include "Engine.h"

#include "MainWindow.h"
#include "Scenes/SolidCubeScene.h"
#include "Scenes/CubeOrderScene.h"
#include "Scenes/ConHexScene.h"
#include "Scenes/ConHexWireScene.h"
#include "Scenes/XMutualScene.h"

Engine::Engine(MainWindow& window) : window(window), graphics(window) {
    scenes.push_back(std::make_unique<SolidCubeScene>());
    scenes.push_back(std::make_unique<CubeOrderScene>());
    scenes.push_back(std::make_unique<ConHexScene>());
    scenes.push_back(std::make_unique<ConHexWireScene>());
    scenes.push_back(std::make_unique<XMutualScene>());
    curScene = scenes.begin();
}

void Engine::UpdateModel() {
    const float dt = 1.0f / 60.0f;

    // Cycle through scenes when tab is pressed
    while (!window.kbd.KeyIsEmpty()) {
        const auto e = window.kbd.ReadKey();

        if (e.GetCode() == VK_TAB && e.IsPress()) {
            CycleScenes();
        }
    }

    // Update scene
    (*curScene)->Update(window.kbd, window.mouse, dt);
}

void Engine::ComposeFrame() {
    graphics.BeginFrame();

    // Draw scene
    (*curScene)->Draw(graphics);

    graphics.EndFrame();
}

void Engine::CycleScenes() {
    if (++curScene == scenes.end()) {
        curScene = scenes.begin();
    }
}
