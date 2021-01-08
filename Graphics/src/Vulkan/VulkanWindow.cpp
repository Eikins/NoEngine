#include <iostream>

#include "Graphics/Vulkan/VulkanWindow.h"

namespace Graphics
{
    GLFWwindow* VulkanWindow::GetHandle() const
    {
        return _handle;
    }

    void VulkanWindow::GLFWErrorCallback(int id, const char* description)
    {
        std::cerr << description << std::endl;
    }

    void VulkanWindow::SetTitle(const std::string& title)
    {
        _descriptor.title = title;
        glfwSetWindowTitle(_handle, title.c_str());
    }

    bool VulkanWindow::ShouldClose()
    {
        return glfwWindowShouldClose(_handle);
    }

    void VulkanWindow::PollEvents()
    {
        glfwPollEvents();
    }

    Math::Vector2 VulkanWindow::GetSize()
    {
        int width, height;
        glfwGetFramebufferSize(_handle, &width, &height);
        return Math::Vector2(width, height);
    }
}