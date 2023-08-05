#include <sstream>

#include "Engine.h"

#include "MainWindow.h"
#include "Scenes/SolidCubeScene.h"
#include "Scenes/CubeOrderScene.h"
#include "Scenes/ConHexScene.h"
#include "Scenes/ConHexWireScene.h"
#include "Scenes/XMutualScene.h"
#include "Scenes/TexCubeScene.h"
#include "Scenes/TexWrapCubeScene.h"
#include "Scenes/FoldedCubeScene.h"
#include "Scenes/FoldedCubeWrapScene.h"
#include "Scenes/CubeSkinnedScene.h"

Engine::Engine(MainWindow& window) : window(window), graphics(window) {
    scenes.push_back(std::make_unique<SolidCubeScene>());
    scenes.push_back(std::make_unique<CubeOrderScene>());
    scenes.push_back(std::make_unique<ConHexScene>());
    scenes.push_back(std::make_unique<ConHexWireScene>());
    scenes.push_back(std::make_unique<XMutualScene>());
    scenes.push_back(std::make_unique<TexCubeScene>());
    scenes.push_back(std::make_unique<TexCubeScene>(2.0f));
    scenes.push_back(std::make_unique<TexWrapCubeScene>(2.0f));
    scenes.push_back(std::make_unique<TexWrapCubeScene>(6.0f));
    scenes.push_back(std::make_unique<TexWrapCubeScene>(L"res\\wood.jpg", 2.0f));
    scenes.push_back(std::make_unique<FoldedCubeScene>());
    scenes.push_back(std::make_unique<FoldedCubeWrapScene>());
    scenes.push_back(std::make_unique<CubeSkinnedScene>(L"res\\dice_skin.png"));
    scenes.push_back(std::make_unique<CubeSkinnedScene>(L"res\\office_skin.jpg"));
    scenes.push_back(std::make_unique<CubeSkinnedScene>(L"res\\office_skin_lores.png"));

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
    (*curScene)->Draw(graphics);

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
    SetWindowTextA(window.GetHWND(), (*curScene)->GetName().c_str());
}
