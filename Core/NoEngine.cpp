// NoEngine.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <thread>

#include "NoEngine.h"
#include "Core/Transform.h"
#include "Graphics/GraphicsContext.h"

#include "Editor/Editor.h"

using namespace std;
using namespace Graphics;
using namespace Math;
using namespace Core;

int main()
{
	try
	{
        Transform root;
        Transform child(&root);
        Transform child2(&child);

        root.SetPosition(Vector3(1, 0, 0));
        child.SetRotation(Quaternion::AxisAngle(Vector3::Up, 90.0f));
        child2.SetPosition(Vector3(1, 0, 0));

        cout << child2.GetLocalToWorldMatrix() << endl;

        WindowDescriptor windowDescriptor = {};
        windowDescriptor.width = 800;
        windowDescriptor.height = 600;
        windowDescriptor.resizable = true;
        windowDescriptor.title = "Vulkan Application";

        Editor::SetupEditor();
        auto graphics = CreateGraphicsContext(windowDescriptor);
        Window& window = graphics.GetWindow();

        while (window.ShouldClose() == false)
        {
            window.PollEvents();
            // scene.Update();
            // graphics.DrawRenderers(scene.GetRenderers(Layer.OPAQUE));
            // graphics.DrawRenderers(scene.GetRenderers(Layer.TRANSPARENT));
            // editors.Update();
            // graphics.DrawEditors();
            // graphics.PresentToScreen();

            graphics.DrawScene(/* root */);
        }

        graphics.Release();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
