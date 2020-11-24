// NoEngine.cpp : Defines the entry point for the application.
//

#include <chrono>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "NoEngine.h"
#include "Scripting/Scripting.h"
#include "Graphics/Window.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/Buffer.h"

#include "Graphics/OpenGL/GLRenderDevice.h"
#include "Graphics/OpenGL/GLWindow.h"

using namespace std;

int main()
{
	std::unique_ptr<Graphics::RenderDevice> renderDevice = make_unique<Graphics::GLRenderDevice>();

	Graphics::WindowDescriptor windowDescriptor;
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
		-1, 0, -1,
		1, 0, -1,
		0, 1, -1,
	};

	Graphics::BufferDescriptor triangleBufferDescriptor;
	triangleBufferDescriptor.size = 3;
	triangleBufferDescriptor.stride = 3 * sizeof(float);

	Graphics::BufferData triangleBufferData;
	triangleBufferData.size = 9;
	triangleBufferData.data = &vertices;

	try
	{

		auto window = renderDevice->CreateWindow(windowDescriptor);
		//auto triangleBuffer = renderDevice->CreateBuffer(triangleBufferDescriptor, triangleBufferData);

		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

		while (window->ShouldClose() == false)
		{

			// Draw Buffer
			// TODO : command queue
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			glEnableClientState(GL_VERTEX_ARRAY);

			//glBindBuffer(GL_ARRAY_BUFFER, triangleBuffer->GetNativeHandle());
			glVertexPointer(3, GL_FLOAT, 0, vertices);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle



			glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeHandle()));

			// Update dt
			auto timePoint = Time::now();
			fsec fs = timePoint - lastFrameTimePoint;
			deltaTime = fs.count();
			time += deltaTime;
			lastFrameTimePoint = timePoint;

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
