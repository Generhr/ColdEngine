#pragma once

#include "FrameTimer.h"
#include "Graphics.h"
#include "Scene.h"

#include <memory>
#include <vector>

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
    void ReverseCycleScenes();
    void SetWindowName() const;

private:
    MainWindow& window;
    Graphics graphics;

    FrameTimer ft;
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<std::unique_ptr<Scene>>::iterator curScene;
};
