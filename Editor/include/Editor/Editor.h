#pragma once

namespace Editor
{
    void CreateContext();
    void DestroyContext();

    bool Enabled();

    void DrawEditors();

    void ShowFPS(float fps);
}