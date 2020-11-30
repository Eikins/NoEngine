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

//#include "Graphics/OpenGL/GLRenderDevice.h"
#include "Graphics/Vulkan/VKRenderDevice.h"

using namespace std;
using namespace Graphics;

int main()
{
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::nanoseconds ns;
	typedef std::chrono::duration<float> fsec;

	std::chrono::steady_clock::time_point lastFrameTimePoint = Time::now();

	float deltaTime = 0.0f;
	float time = 0.0f;
	float nextTick = 0.0f;

	// ==== WINDOW ===================================================
	WindowDescriptor windowDescriptor;
	windowDescriptor.width = 800;
	windowDescriptor.height = 600;
	windowDescriptor.title = "NoEngine (Vulkan)";
	windowDescriptor.resizable = false;
	// ===============================================================

	auto renderDevice = VKRenderDevice::CreateDevice(windowDescriptor);

	//// ==== TRIANGLE BUFFER ==========================================
	//float vertices[] = {
	//	-1, 0, 0, 1, 0, 0,
	//	1, 0, 0, 0, 1, 0,
	//	0, 1, 0, 0, 0, 1
	//};

	//BufferDescriptor triangleBufferDescriptor;
	//triangleBufferDescriptor.size = 3;
	//triangleBufferDescriptor.stride = 6 * sizeof(float);

	//BufferData triangleBufferData;
	//triangleBufferData.size = 18;
	//triangleBufferData.data = &vertices;
	//// ===============================================================

	// ==== VERTEX SHADER ============================================
	ShaderDescriptor vertexDescriptor;
	vertexDescriptor.shaderType = ShaderType::VERTEX;

	ShaderCreationDescriptor vertexCreateInfo;
	vertexCreateInfo.filename = "Shaders/SPIR-V/triangle.vert.spv";
	vertexCreateInfo.pDesc = &vertexDescriptor;
	// ===============================================================

	// ==== PIXEL SHADER =============================================
	ShaderDescriptor fragDescriptor;
	fragDescriptor.shaderType = ShaderType::PIXEL;

	ShaderCreationDescriptor fragCreateInfo;
	fragCreateInfo.filename = "Shaders/SPIR-V/triangle.frag.spv";
	fragCreateInfo.pDesc = &fragDescriptor;
	// ===============================================================

	try
	{
		auto window = renderDevice->GetWindow();
		auto swapChain = renderDevice->CreateSwapChain();
		auto vertexShader = renderDevice->CreateShader(vertexCreateInfo);
		auto fragmentShader = renderDevice->CreateShader(fragCreateInfo);
		//auto triangleBuffer = renderDevice->CreateBuffer(triangleBufferDescriptor, triangleBufferData);


		Viewport viewport;
		BlendStateDescriptor blendState;
		RasterizerStateDescriptor rasterizerState;
		DepthStencilStateDescriptor depthStencilState;

		viewport.width = windowDescriptor.width;
		viewport.height = windowDescriptor.height;
		blendState.enabled = true;

		PipelineStateCreationDescriptor pipelineCreateInfo;
		pipelineCreateInfo.pSwapChain = swapChain;
		pipelineCreateInfo.pVertexShader = vertexShader;
		pipelineCreateInfo.vertexEntrypoint = "main";
		pipelineCreateInfo.pPixelShader = fragmentShader;
		pipelineCreateInfo.pixelEntrypoint = "main";
		pipelineCreateInfo.viewport = viewport;
		pipelineCreateInfo.blendState = blendState;
		pipelineCreateInfo.rasterizerState = rasterizerState;
		pipelineCreateInfo.depthStencilState = depthStencilState;

		auto pipelineState = renderDevice->CreatePipelineState(pipelineCreateInfo);

		//auto commandBuffer = renderDevice->CreateCommandBuffer();

		//// VAO...
		//GLuint vao;
		//glGenVertexArrays(1, &vao);
		//glBindVertexArray(vao);

		//auto positionAttribute = glGetAttribLocation(pipelineState->GetNativeHandle(), "inPosition");
		//auto colorAttribute = glGetAttribLocation(pipelineState->GetNativeHandle(), "inColor");

		//glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, NULL);
		//glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*) (sizeof(float) * 3));
		//glEnableVertexAttribArray(positionAttribute);
		//glEnableVertexAttribArray(colorAttribute);


		while (window->ShouldClose() == false)
		{
		//	pipelineState->Bind();
		//	// TODO : Replace OpenGL code with commands
		//	commandBuffer->Clear(true, true, 0.1f, 0.1f, 0.1f, 1.0f, 1.0f);
		//	// commandBuffer->Draw(buffer)
		//	glBindVertexArray(vao);
		//	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		//	// commandBuffer->Swap()
		//	glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeHandle()));
		//	// deviceContext->ExecuteCommandBuffer(commandBuffer)

		//	// Update dt
		//	auto timePoint = Time::now();
		//	fsec fs = timePoint - lastFrameTimePoint;
		//	deltaTime = fs.count();
		//	time += deltaTime;
		//	lastFrameTimePoint = timePoint;

		//	// 60 FPS lock
		//	if (deltaTime < 1.0f / 60.0f)
		//	{
		//		long sleepDuration = ((1.0f / 60.0f) - deltaTime) * 1000000000;
		//		std::this_thread::sleep_for(std::chrono::nanoseconds(sleepDuration));
		//		deltaTime = 1.0f / 60.0f;
		//	}
			window->PollEvents();
		}

		pipelineState->Release();

		vertexShader->Release();
		fragmentShader->Release();

		swapChain->Release();
		renderDevice->Release();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
