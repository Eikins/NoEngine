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
        glfwSetWindowTitle(static_cast<GLFWwindow*>(_handle), title.c_str());
    }

    bool VulkanWindow::ShouldClose()
    {
        return glfwWindowShouldClose(static_cast<GLFWwindow*>(_handle));
    }

    void VulkanWindow::PollEvents()
    {
        glfwPollEvents();
    }
}