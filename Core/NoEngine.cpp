// NoEngine.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <thread>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "NoEngine.h"

#include "Graphics/Vulkan/VulkanApplication.h"
//#include "Scripting/Scripting.h"
//#include "Graphics/Window.h"
//#include "Graphics/RenderDevice.h"
//#include "Graphics/Buffer.h"
//#include "Graphics/Shader.h"

//#include "Graphics/OpenGL/GLRenderDevice.h"
//#include "Graphics/Vulkan/VKRenderDevice.h"

using namespace std;
using namespace Graphics;

int main()
{
	try
	{
		VulkanApplication vkApp;
		vkApp.Run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
