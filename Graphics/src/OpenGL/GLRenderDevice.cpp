#include "gl/glew.h"

#include "Graphics/OpenGL/GLRenderDevice.h"
#include "Graphics/OpenGL/GLWindow.h"
#include "Graphics/OpenGL/GLBuffer.h"

namespace Graphics
{

	std::unique_ptr<Window> GLRenderDevice::CreateWindow(const WindowDescriptor& descriptor)
	{
		glfwSetErrorCallback(&GLWindow::GLFWErrorCallback);
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, descriptor.resizable ? GLFW_TRUE : GLFW_FALSE);

		auto window = std::make_unique<GLWindow>();
		window->_handle = glfwCreateWindow(descriptor.width, descriptor.height, descriptor.title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(static_cast<GLFWwindow*>(window->_handle));
		glewInit();
		return window;
	}


	std::unique_ptr<Buffer> GLRenderDevice::CreateBuffer(const BufferDescriptor &descriptor, const BufferData &data)
	{
		auto buffer = std::make_unique<GLBuffer>();
		glGenBuffers(1, &(buffer->GLID));
		glBindBuffer(GL_ARRAY_BUFFER, buffer->GLID);
		glBufferData(GL_ARRAY_BUFFER, descriptor.size * descriptor.stride, data.data, GL_STATIC_DRAW);

		return buffer;
	}
}