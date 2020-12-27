#pragma once

#include "Components/CameraEditor.hpp"

namespace Editor
{
    void CreateContext();
    void DestroyContext();

    bool Enabled();

    void DrawEditors();

    void ShowFPS(float fps);
    void DrawInspector(Core::Camera& camera);
}