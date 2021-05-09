#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Graphics/Window.h"

namespace Graphics
{
    class VulkanWindow : public Window
    {
    private:
        GLFWwindow* _handle;
    public:
        VulkanWindow(GLFWwindow* handle) : _handle(handle) {}

        virtual void SetTitle(const std::string& title) override;
        virtual bool ShouldClose() override;
        virtual void PollEvents() override;

        virtual Math::Vector2 GetSize() override;

        GLFWwindow* GetHandle() const;

        static void GLFWErrorCallback(int id, const char* description);
    };
}