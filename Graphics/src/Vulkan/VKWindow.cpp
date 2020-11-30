#include <iostream>

#include <GLFW/glfw3.h>

#include "Graphics/Vulkan/VKWindow.h"

namespace Graphics
{
	void VKWindow::GLFWErrorCallback(int id, const char* description)
	{
		std::cerr << description << std::endl;
	}

	void VKWindow::SetTitle(const std::string& title)
	{
		_descriptor.title = title;
		glfwSetWindowTitle(static_cast<GLFWwindow*>(_handle), title.c_str());
	}

	bool VKWindow::ShouldClose()
	{
		return glfwWindowShouldClose(static_cast<GLFWwindow*>(_handle));
	}

	void VKWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void VKWindow::Release()
	{
		glfwDestroyWindow(_handle);
	}

	void* VKWindow::GetNativeHandle()
	{
		return _handle;
	}

}