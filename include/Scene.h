#pragma once

#include <string>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class Scene {
public:
    Scene(const std::string& name) : name(name) {
    }

    virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) = 0;

    virtual void Draw(Graphics& graphics) const = 0;

    const std::string& GetName() const {
        return name;
    }

private:
    std::string name;
};
