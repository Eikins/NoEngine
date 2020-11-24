// NoEngine.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <thread>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "NoEngine.h"
#include "Scripting/Scripting.h"
#include "Graphics/Window.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/Buffer.h"
#include "Graphics/Shader.h"

#include "Graphics/OpenGL/GLRenderDevice.h"
#include "Graphics/OpenGL/GLWindow.h"

using namespace std;
using namespace Graphics;

int main()
{
	std::unique_ptr<Graphics::RenderDevice> renderDevice = make_unique<Graphics::GLRenderDevice>();

	WindowDescriptor windowDescriptor;
	windowDescriptor.width = 800;
	windowDescriptor.height = 600;
	windowDescriptor.title = "NoEngine (OpenGL)";
	windowDescriptor.resizable = false;

	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::nanoseconds ns;
	typedef std::chrono::duration<float> fsec;

	std::chrono::steady_clock::time_point lastFrameTimePoint = Time::now();

	float deltaTime = 0.0f;
	float time = 0.0f;
	float nextTick = 0.0f;

	struct TriangleVertexData
	{
		float position[3];
	};

	GLfloat vertices[] = {
		-1, 0, 0,
		1, 0, 0,
		0, 1, 0,
	};

	BufferDescriptor triangleBufferDescriptor;
	triangleBufferDescriptor.size = 3;
	triangleBufferDescriptor.stride = 3 * sizeof(float);

	BufferData triangleBufferData;
	triangleBufferData.size = 9;
	triangleBufferData.data = &vertices;

	// ==== SHADERS ====
	ShaderDescriptor vertexDescriptor;
	vertexDescriptor.shaderType = ShaderType::VERTEX;
	ShaderCreationDescriptor vertexCreateInfo;
	vertexCreateInfo.filename = "Shaders/GLSL/triangle.vert.glsl";
	vertexCreateInfo.pDesc = &vertexDescriptor;

	ShaderDescriptor fragDescriptor;
	fragDescriptor.shaderType = ShaderType::PIXEL;
	ShaderCreationDescriptor fragCreateInfo;
	fragCreateInfo.filename = "Shaders/GLSL/triangle.frag.glsl";
	fragCreateInfo.pDesc = &fragDescriptor;

	try
	{

		auto window = renderDevice->CreateWindow(windowDescriptor);
		auto vertexShader = renderDevice->CreateShader(vertexCreateInfo);
		auto fragmentShader = renderDevice->CreateShader(fragCreateInfo);
		auto triangleBuffer = renderDevice->CreateBuffer(triangleBufferDescriptor, triangleBufferData);


		// Create a program
		auto shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader->GetNativeHandle());
		glAttachShader(shaderProgram, fragmentShader->GetNativeHandle());
		glLinkProgram(shaderProgram);

		// VAO...
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		auto positionAttribute = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(positionAttribute);

		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

		while (window->ShouldClose() == false)
		{

			// Draw Buffer
			// TODO : command queue
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glBindVertexArray(vao);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

			//glColor3f(1.0f, 0.0f, 0.0f);
			//glBegin(GL_TRIANGLES);
			//glVertex2f(-1.0f, 0.0f);
			//glVertex2f(1.0f, 0.0f);
			//glVertex2f(0.0f, 1.0f);
			//glEnd();


			glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeHandle()));

			// Update dt
			auto timePoint = Time::now();
			fsec fs = timePoint - lastFrameTimePoint;
			deltaTime = fs.count();
			time += deltaTime;
			lastFrameTimePoint = timePoint;


			// 60 FPS fixed
			if (deltaTime < 1.0f / 60.0f)
			{
				long sleepDuration = ((1.0f / 60.0f) - deltaTime) * 1000000000;
				std::this_thread::sleep_for(std::chrono::nanoseconds(sleepDuration));
				deltaTime = 1.0f / 60.0f;
			}

			window->PollEvents();
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
