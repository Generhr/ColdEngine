#pragma once

#include <memory>
#include <vector>

#include "Graphics.h"
#include "Scene.h"

class Engine {
public:
    explicit Engine(class MainWindow& window);

    ~Engine() = default;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    void UpdateModel();
    void ComposeFrame();

    void CycleScenes();

private:
    MainWindow& window;
    Graphics graphics;

    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<std::unique_ptr<Scene>>::iterator curScene;
};
