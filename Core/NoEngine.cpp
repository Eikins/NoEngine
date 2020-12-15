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

#include "Core/Transform.h"

using namespace std;
using namespace Graphics;
using namespace Math;
using namespace Core;

int main()
{
	try
	{

        //auto T = Vector3(0, 2, 4);
        //auto R = Quaternion::AxisAngle(Vector3(0, 1, 0), 45.0);
        //auto S = Vector3(2.5, -1, 1);

        //std::cout << Matrix4x4::TRS(T, R, S) << std::endl;

        Transform* root = new Transform();
        Transform* child = new Transform(root);
        Transform* child2 = new Transform(child);

        root->SetPosition(Vector3(1, 0, 0));
        child->SetRotation(Quaternion::AxisAngle(Vector3::Up, 90.0f));
        child2->SetPosition(Vector3(1, 0, 0));

        child2->SetParent(nullptr);

        std::cout << child2->GetLocalToWorldMatrix() << std::endl;

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
