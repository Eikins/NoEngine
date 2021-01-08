#pragma once

#include <memory>

#include "Window.h"
#include "Core/Components/Camera.h"
#include "Core/Components/Renderer.h"

namespace Graphics
{
#pragma region Forward Dependency Shield
    // Used to avoid forward dependencies with vulkan and glfw
    class GraphicsContextImpl;
#pragma endregion

    class GraphicsContext {
    private:
        GraphicsContext();
        GraphicsContextImpl* _impl = nullptr;
        Window* _window = nullptr;
    public:
        Window& GetWindow();

        void PrepareRenderers(std::vector<Core::Renderer>& renderers);

        void SetupCameraProperties(Core::Camera& camera);

        bool BeginFrame();
        void EndFrame();
        void DrawRenderers(std::vector<Core::Renderer>& renderers);

        void BeginEditorFrame();
        void EndEditorFrame();

        void RenderAsync();
        void WaitForRenderCompletion();

        void DrawScene();
        void Release();

        friend GraphicsContext CreateGraphicsContext(const WindowDescriptor& windowDescriptor);
    };
}