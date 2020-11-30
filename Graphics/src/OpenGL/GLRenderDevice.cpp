#include <vector>
#include <fstream>
#include <iostream>

#include "gl/glew.h"

#include "Graphics/OpenGL/GLRenderDevice.h"
#include "Graphics/OpenGL/GLWindow.h"
#include "Graphics/OpenGL/GLBuffer.h"
#include "Graphics/OpenGL/GLCommandBuffer.h"
#include "Graphics/OpenGL/GLShader.h"
#include "Graphics/OpenGL/GLPipelineState.h"

namespace Graphics
{
	std::vector<char> ReadFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (file.is_open() == false)
		{
			throw std::runtime_error("Failed to open file");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char>buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	RenderDevice* GLRenderDevice::CreateDevice(const WindowDescriptor& descriptor)
	{
		auto renderDevice = new GLRenderDevice();
		auto window = new GLWindow();
		glfwSetErrorCallback(&GLWindow::GLFWErrorCallback);
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, descriptor.resizable ? GLFW_TRUE : GLFW_FALSE);

		window->_handle = glfwCreateWindow(descriptor.width, descriptor.height, descriptor.title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(static_cast<GLFWwindow*>(window->_handle));
		glewInit();

		renderDevice->_window = window;
		return renderDevice;
	}

	Window* GLRenderDevice::GetWindow()
	{
		return _window;
	}

	Buffer* GLRenderDevice::CreateBuffer(const BufferDescriptor &descriptor, const BufferData &data)
	{
		auto buffer = new GLBuffer();
		glGenBuffers(1, &(buffer->GLID));
		glBindBuffer(GL_ARRAY_BUFFER, buffer->GLID);
		glBufferData(GL_ARRAY_BUFFER, descriptor.size * descriptor.stride, data.data, GL_STATIC_DRAW);

		return buffer;
	}

	Shader* GLRenderDevice::CreateShader(const ShaderCreationDescriptor &descriptor)
	{
		auto shader = new GLShader();
		auto buffer = ReadFile(descriptor.filename);

		const char *shaderSource = &buffer[0];

		GLenum glShaderType;
		switch (descriptor.pDesc->shaderType)
		{
		case ShaderType::VERTEX:
			glShaderType = GL_VERTEX_SHADER;
			break;
		case ShaderType::PIXEL:
			glShaderType = GL_FRAGMENT_SHADER;
			break;
		default:
			glShaderType = GL_NONE;
		}

		if (glShaderType != GL_NONE)
		{
			auto glid = glCreateShader(glShaderType);
			glShaderSource(glid, 1, &shaderSource, NULL);
			glCompileShader(glid);

			GLint test;
			glGetShaderiv(glid, GL_COMPILE_STATUS, &test);
			if (!test)
			{
				std::cerr << "Shader compilation failed with this message:" << std::endl;
				std::vector<char> compilation_log(512);
				glGetShaderInfoLog(glid, compilation_log.size(), NULL, &compilation_log[0]);
				std::cerr << &compilation_log[0] << std::endl;
			}
			shader->GLID = glid;
		}

		return shader;
	}

	SwapChain* GLRenderDevice::CreateSwapChain()
	{
		return nullptr;
	}

	PipelineState* GLRenderDevice::CreatePipelineState(const PipelineStateCreationDescriptor &descriptor)
	{
		auto pipelineState = new GLPipelineState();
		auto glid = glCreateProgram();
		glAttachShader(glid, *static_cast<GLuint*>(descriptor.pVertexShader->GetNativeHandle()));
		glAttachShader(glid, *static_cast<GLuint*>(descriptor.pPixelShader->GetNativeHandle()));
		glLinkProgram(glid);
		pipelineState->GLID = glid;
		return pipelineState;
	}

	CommandBuffer* GLRenderDevice::CreateCommandBuffer()
	{
		return new GLCommandBuffer();
	}

#pragma region DeviceObject Methods
	void GLRenderDevice::Release() {}
	void* GLRenderDevice::GetNativeHandle() { return nullptr; }
#pragma endregion

}