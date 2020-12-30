// NoEngine.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <thread>

#include "NoEngine.h"
#include "Core/Transform.h"
#include "Core/Components/Camera.h"
#include "Core/Components/Renderer.h"
#include "Core/Assets/Mesh.h"
#include "Graphics/GraphicsContext.h"

#ifdef NoEngine_Editor
#include "Editor/Editor.h"
#include "imgui.h"
#endif

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

#ifdef NoEngine_Editor
        Editor::CreateContext();
        Editor::SetEditorScale(2);
        int selectionSceneIndex = -1;
#endif
        auto graphics = CreateGraphicsContext(windowDescriptor);
        Window& window = graphics.GetWindow();

        Time::deltaTime = 1.0f / 60.0f;

        // ==========================================================================
        // Temporary scene setup
        // Assets
        auto standardVertexShader = Shader::LoadFromFile("Standard/Vertex", "Shaders/SPIR-V/standard.vert.spv", "main");
        auto standardFragmentShader = Shader::LoadFromFile("Standard/Fragment", "Shaders/SPIR-V/standard.frag.spv", "main");

        auto barracudaMeshes = Mesh::LoadFromFile("Models/Barracuda/Barracuda_Model.fbx");
        Material barracudaMaterial("Standard", &standardVertexShader, &standardFragmentShader);

#pragma region Sin Wave Script
        class SinWaveScript : public Script
        {
        public:
            SinWaveScript() : Script("SinWave") {}
        private:
            float _speed = 1.0f;

            virtual void Initialize(GameObject* gameObject, Transform* transform) override {}

            virtual void OnUpdate(GameObject* gameObject, Transform* transform) override
            {
                transform->SetPosition(Vector3(0, Sin(Time::time * _speed), 0));
            }

            virtual void DrawExposedProperties() override
            {
                ImGui::InputFloat("Speed", &_speed);
            }

        } sinWaveScript;
#pragma endregion


        // Game Object
        Scene scene("Example Scene");
        GameObject& cameraObject = scene.CreateGameObject("Main Camera", nullptr);
        GameObject& emptyObject = scene.CreateGameObject("Empty", nullptr);
        GameObject& barracudaObject = scene.CreateGameObject("Barracuda", &emptyObject.GetTransform());

        // Components
        Camera& mainCamera = static_cast<Camera&>(cameraObject.AddComponent(ComponentType::Camera));
        mainCamera.SetAspectRatio(static_cast<float>(windowDescriptor.width) / windowDescriptor.height);
        Renderer& barracudaRenderer = static_cast<Renderer&>(barracudaObject.AddComponent(ComponentType::Renderer));
        barracudaRenderer.mesh = &barracudaMeshes[0];
        barracudaRenderer.material = &barracudaMaterial;
        ScriptedBehaviour& barracudaBehaviour = static_cast<ScriptedBehaviour&>(barracudaObject.AddComponent(ComponentType::ScriptedBehaviour));
        barracudaBehaviour.script = &sinWaveScript;

        // Set Transforms
        emptyObject.GetTransform().SetScale(Vector3(0.2f, 0.2f, 0.2f));
        cameraObject.GetTransform().SetPosition(Vector3(0, 0, -10));
        // ==========================================================================
        std::vector<Renderer*> renderers;
        scene.GetRenderers(RenderingLayer::Opaque, renderers);
        graphics.PrepareRenderers(renderers);

        while (window.ShouldClose() == false)
        {
            window.PollEvents();
            graphics.SetupCameraProperties(mainCamera);
            if (graphics.BeginFrame())
            {
                graphics.DrawRenderers(renderers);
#ifdef NoEngine_Editor
                if (Editor::Enabled())
                {
                    graphics.BeginEditorFrame();
                    Editor::DrawSceneHierarchy(scene, &selectionSceneIndex);
                    if (selectionSceneIndex >= 0)
                    {
                        Editor::DrawInspector(&scene.GetGameObjectAtIndex(selectionSceneIndex));
                    }
                    else
                    {
                        Editor::DrawInspector(nullptr);
                    }
                    Editor::ShowFPS(1.0f / Time::deltaTime);
                    graphics.EndEditorFrame();
                }
#endif
                graphics.EndFrame();
                graphics.RenderAsync();
                scene.Update();
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
