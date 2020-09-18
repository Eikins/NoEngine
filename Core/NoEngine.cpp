// NoEngine.cpp : Defines the entry point for the application.
//

#include "NoEngine.h"
#include "Graphics/VulkanApplication.h"

using namespace std;

int main()
{
	Graphics::VulkanApplication app;

	try
	{
		app.Run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
