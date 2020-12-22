#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanWindow.h"
#include "Graphics/Vulkan/VulkanRenderer.h"

#include "Graphics/Vulkan/VulkanImGuiRenderer.h"

namespace Graphics
{
#pragma region Graphics Context Implementation Prototype
    class GraphicsContextImpl
    {
    private:
        GLFWwindow* _window;
        VulkanContext _vkContext;
        VulkanRenderer _vkRenderer;
        VulkanImGuiRenderer _imGuiRenderer;

        void CreateWindow(const WindowDescriptor& descriptor);

        friend GraphicsContext;
        friend GraphicsContext CreateGraphicsContext(const WindowDescriptor& windowDescriptor);
    };
#pragma endregion

#pragma region Graphics Context
    GraphicsContext::GraphicsContext() {}

    Window& GraphicsContext::GetWindow()
    {
        return *_window;
    }

    void GraphicsContext::DrawScene()
    {
        //_impl->_vkRenderer.DrawFrame();
        _impl->_imGuiRenderer.Draw();
    }

    void GraphicsContext::Release()
    {
        vkDeviceWaitIdle(_impl->_vkContext.device);

        _impl->_vkRenderer.Cleanup();
        _impl->_imGuiRenderer.Release();
        _impl->_vkContext.Release();

        glfwDestroyWindow(_impl->_vkRenderer._window);
        glfwTerminate();
    }

    GraphicsContext Graphics::CreateGraphicsContext(const WindowDescriptor& descriptor)
    {
        GraphicsContext context;
        context._impl = new GraphicsContextImpl();
        context._impl->CreateWindow(descriptor);

        auto vkWindow = new VulkanWindow(context._impl->_window);
        auto vkContext = VulkanContext::CreateVulkanContext(*vkWindow);

        context._window = vkWindow;
        context._impl->_vkContext = vkContext;
        context._impl->_vkRenderer._window = vkWindow->GetHandle();
        context._impl->_vkRenderer.Init(vkContext);
        context._impl->_imGuiRenderer.Init(vkWindow->GetHandle(), vkContext);

        return context;
    }
#pragma endregion

#pragma region Vulkan Graphics Context Definitions
    void GraphicsContextImpl::CreateWindow(const WindowDescriptor& descriptor)
    {
        glfwSetErrorCallback(VulkanWindow::GLFWErrorCallback);
        glfwInit();

        // Using Vulkan, do not create OpenGL Context here
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, descriptor.resizable);

        _window = glfwCreateWindow(descriptor.width, descriptor.height, descriptor.title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(_window, this);
        if (descriptor.resizable)
        {
            glfwSetFramebufferSizeCallback(_window, _vkRenderer.FramebufferResizeCallback);
        }
    }
#pragma endregion
}