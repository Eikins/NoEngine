#pragma once

#include <memory>

#include "Window.h"

namespace Graphics
{
#pragma region Forward Dependency Shield
    // Used to avoid forward dependencies with vulkan and glfw
    class GraphicsContextImpl;
#pragma endregion

    class GraphicsContext {
    private:
        GraphicsContext();
        GraphicsContextImpl* _impl;
        Window* _window;
    public:
        Window& GetWindow();

        void DrawScene();
        void Release();

        friend GraphicsContext CreateGraphicsContext(const WindowDescriptor& windowDescriptor);
    };
}