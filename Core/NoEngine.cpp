// NoEngine.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <thread>

#include "NoEngine.h"
#include "Core/Transform.h"
#include "Graphics/GraphicsContext.h"

#include "Editor/Editor.h"

#include "Core/Time.h"

using namespace std;
using namespace Graphics;
using namespace Math;
using namespace Core;

int main()
{
    typedef std::chrono::high_resolution_clock Chrono;
    typedef std::chrono::nanoseconds ns;
    typedef std::chrono::duration<float> fsec;

    std::chrono::steady_clock::time_point lastFrameTimePoint = Chrono::now();

	try
	{
        //Transform root;
        //Transform child(&root);
        //Transform child2(&child);

        //root.SetPosition(Vector3(1, 0, 0));
        //child.SetRotation(Quaternion::AxisAngle(Vector3::Up, 90.0f));
        //child2.SetPosition(Vector3(1, 0, 0));

        //cout << child2.GetLocalToWorldMatrix() << endl;

        WindowDescriptor windowDescriptor = {};
        windowDescriptor.width = 1920;
        windowDescriptor.height = 1080;
        windowDescriptor.resizable = true;
        windowDescriptor.title = "Vulkan Application";

        Editor::CreateContext();
        auto graphics = CreateGraphicsContext(windowDescriptor);
        Window& window = graphics.GetWindow();

        Time::deltaTime = 1.0f / 60.0f;

        // Temporary camera setup
        Transform cameraTransform;
        Camera mainCamera;
        mainCamera._transform = &cameraTransform;

        while (window.ShouldClose() == false)
        {
            window.PollEvents();
            graphics.SetupCameraProperties(mainCamera);
            if (graphics.BeginFrame())
            {
                //graphics.DrawRenderers();
                //graphics.DrawRenderers(scene.GetRenderers(Layer.TRANSPARENT));

                if (Editor::Enabled())
                {
                    graphics.BeginEditorFrame();
                    Editor::DrawInspector(mainCamera);
                    Editor::ShowFPS(1.0f / Time::deltaTime);
                    graphics.EndEditorFrame();
                }

                graphics.EndFrame();
                graphics.RenderAsync();

                //scene.Update();
                
                graphics.WaitForRenderCompletion();

                // Update dt
	            auto timePoint = Chrono::now();
	            fsec fs = timePoint - lastFrameTimePoint;
	            Time::deltaTime = fs.count();
	            Time::time += Time::deltaTime;
	            lastFrameTimePoint = timePoint;

	            // 60 FPS lock
	            if (Time::deltaTime < 1.0f / 60.0f)
	            {
		            long sleepDuration = ((1.0f / 60.0f) - Time::deltaTime) * 1000000000;
		            std::this_thread::sleep_for(std::chrono::nanoseconds(sleepDuration));
		            Time::deltaTime = 1.0f / 60.0f;
	            }
            }
        }

        graphics.Release();
        Editor::DestroyContext();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
