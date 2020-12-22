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
		_descriptor.title = title;
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

	void GLWindow::Release()
	{
		glfwDestroyWindow(_handle);
	}

	void* GLWindow::GetNativeHandle()
	{
		return _handle;
	}

}