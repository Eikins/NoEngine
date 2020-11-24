#include <iostream>

#include <GLFW/glfw3.h>

#include "Graphics/OpenGL/GLWindow.h"

namespace Graphics
{
	void GLWindow::GLFWErrorCallback(int id, const char* description)
	{
		std::cerr << description << std::endl;
	}

	void GLWindow::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(static_cast<GLFWwindow*>(_handle), title.c_str());
	}

	bool GLWindow::ShouldClose()
	{
		return glfwWindowShouldClose(static_cast<GLFWwindow*>(_handle));
	}

	void GLWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void* GLWindow::GetNativeHandle()
	{
		return _handle;
	}

}