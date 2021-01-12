// NoEngine.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <thread>

#include "NoEngine.h"
#include "Core/Transform.h"
#include "Core/Components/Camera.h"
#include "Core/Components/Renderer.h"
#include "Core/Components/AABBCollider.h"
#include "Core/Components/ScriptedBehaviour.h"
#include "Core/Components/Rigidbody.h"
#include "Core/Components/Light.h"

#include "Core/Systems/PhysicsSystem.h"
#include "Core/Systems/GraphicsSystem.h"
#include "Core/Systems/ScriptSystem.h"

#include "Core/Assets/Mesh.h"
#include "Graphics/GraphicsContext.h"

#ifdef NoEngine_Editor
#include "Editor/Editor.h"
#include "imgui.h"
#endif

#include "Core/Time.h"

#include "Core/Modules/GameManager.hpp"

using namespace std;
using namespace Graphics;
using namespace Math;
using namespace Core;

typedef std::chrono::high_resolution_clock Chrono;
typedef std::chrono::nanoseconds ns;
typedef std::chrono::duration<float> fsec;

class NoEngine
{
    GameManager _gameManager;
    PhysicsSystem* _physics = nullptr;
    CollisionSystem* _collisionWorld = nullptr;
    GraphicsSystem* _graphics;
    ScriptSystem* _scripts;

    InputMaster _inputMaster;

    Camera* _mainCamera;

    void RegisterComponents()
    {
        _gameManager.RegisterComponent<Camera>();
        _gameManager.RegisterComponent<Renderer>();
        _gameManager.RegisterComponent<AABBCollider>();
        _gameManager.RegisterComponent<ScriptedBehaviour>();
        _gameManager.RegisterComponent<Rigidbody>();
        _gameManager.RegisterComponent<Light>();
    }

    void RegisterSystems()
    {
        _collisionWorld = _gameManager.RegisterSystem<CollisionSystem>();
        _physics = _gameManager.RegisterSystem<PhysicsSystem>();
        _graphics = _gameManager.RegisterSystem<GraphicsSystem>();
        _scripts = _gameManager.RegisterSystem<ScriptSystem>();

        _gameManager.SetSystemSignature<CollisionSystem>(_collisionWorld->CreateSignature());
        _gameManager.SetSystemSignature<PhysicsSystem>(_physics->CreateSignature());
        _gameManager.SetSystemSignature<GraphicsSystem>(_graphics->CreateSignature());
        _gameManager.SetSystemSignature<ScriptSystem>(_scripts->CreateSignature());

        _physics->BindCollisionWorld(_collisionWorld);
    }

public:
    void Setup()
    {
        _gameManager.Init();
        RegisterComponents();
        RegisterSystems();
    }

    int Run()
    {
        // Load Scene
        Scene scene("Example Scene");

        // Meshes
        auto barracudaMeshes = Mesh::LoadFromFile("Models/Barracuda/Barracuda_Model.fbx");
        auto clownfishMeshes = Mesh::LoadFromFile("Models/Clownfish/Clownfish_Model.fbx");

        // Shaders
        auto standardVertexShader = Shader::LoadFromFile("Standard/Vertex", "Shaders/SPIR-V/standard.vert.spv", "main");
        auto standardFragmentShader = Shader::LoadFromFile("Standard/Fragment", "Shaders/SPIR-V/standard.frag.spv", "main");
        auto clownfishFragmentShader = Shader::LoadFromFile("Clownfish/Fragment", "Shaders/SPIR-V/clownfish.frag.spv", "main");

        // Materials
        Material barracudaMaterial("Standard", &standardVertexShader, &standardFragmentShader);
        RenderStateBlock renderStateBlock = RenderStateBlock::DefaultOpaque;
        //renderStateBlock.depthTestEnabled = false;
        Material clownfishMaterial("Clownfish", &standardVertexShader, &clownfishFragmentShader, renderStateBlock,
        {
            {"Color", MaterialPropertyType::COLOR},
            {"IndirectColor", MaterialPropertyType::COLOR}
        });


        Script sinWaveScript("SinWave", "Scripts/SinWave.cs");

        // Create Game Objects
        GameObject* cameraObject = _gameManager.CreateGameObject("Main Camera");
        GameObject* mainLightObject = _gameManager.CreateGameObject("Main Light");
        GameObject* emptyObject = _gameManager.CreateGameObject("Empty");
        GameObject* barracudaObject = _gameManager.CreateGameObject("Barracuda", emptyObject->GetTransform());
        GameObject* clownfishObject = _gameManager.CreateGameObject("Clownfish");
        GameObject* groundObject = _gameManager.CreateGameObject("Ground");

        scene.rootTransforms.push_back(cameraObject->GetTransform());
        scene.rootTransforms.push_back(mainLightObject->GetTransform());
        scene.rootTransforms.push_back(emptyObject->GetTransform());
        scene.rootTransforms.push_back(clownfishObject->GetTransform());
        scene.rootTransforms.push_back(groundObject->GetTransform());

        // Components
        _mainCamera = &_gameManager.AddComponent<Camera>(cameraObject);

        auto& light = _gameManager.AddComponent<Light>(mainLightObject);
        light._type = LightType::DIRECTIONAL;

        auto& barracudaRenderer = _gameManager.AddComponent<Renderer>(barracudaObject);
        auto& barracudaAABB = _gameManager.AddComponent<AABBCollider>(barracudaObject);
        auto& barracudaScript = _gameManager.AddComponent<ScriptedBehaviour>(barracudaObject);
        auto& barracudaRb = _gameManager.AddComponent<Rigidbody>(barracudaObject);
        barracudaRenderer.mesh = &barracudaMeshes[0];
        barracudaRenderer.material = &barracudaMaterial;
        barracudaAABB.bounds = Bounds(Vector3(0, 0.7f, 0), Vector3(10, 2, 1));
        barracudaScript.script = &sinWaveScript;

        auto& clownfishRenderer = _gameManager.AddComponent<Renderer>(clownfishObject);
        auto& clownfishAABB = _gameManager.AddComponent<AABBCollider>(clownfishObject);
        _gameManager.AddComponent<Rigidbody>(clownfishObject);
        clownfishRenderer.mesh = &clownfishMeshes[0];
        clownfishRenderer.material = &clownfishMaterial;
        clownfishAABB.bounds = Bounds(Vector3::Zero, Vector3::One * 5);

        auto& groundAABB = _gameManager.AddComponent<AABBCollider>(groundObject);
        groundAABB.bounds = Bounds(Vector3::Zero, Vector3(20, 2, 20));

        // Set Transforms
        emptyObject->GetTransform()->SetScale(Vector3::One * 0.2f);
        cameraObject->GetTransform()->SetPosition(Vector3(0, 0, -10));
        clownfishObject->GetTransform()->SetPosition(Vector3(-5, 0, 0));
        clownfishObject->GetTransform()->SetScale(Vector3::One * 0.15f);
        groundObject->GetTransform()->SetPosition(Vector3(0, -10, 0));

        _gameManager.SetScene(&scene);

        _scripts->InitRuntime();
        _scripts->CompileAndLoadCore();
        _scripts->CompileAndLoadScripts();

        try
        {
            WindowDescriptor windowDescriptor = {};
            windowDescriptor.width = 1920;
            windowDescriptor.height = 1080;
            windowDescriptor.resizable = true;
            windowDescriptor.title = "Vulkan Application";
            _mainCamera->SetAspectRatio(static_cast<float>(windowDescriptor.width) / windowDescriptor.height);

#ifdef NoEngine_Editor
            Editor::CreateContext(_gameManager);
            Editor::SetEditorScale(2);
#endif

            _graphics->CreateContext(windowDescriptor);
            _graphics->BindInputMaster(&_inputMaster);

            Window& window = _graphics->GetWindow();

            Time::deltaTime = 1.0f / 60.0f;
            std::chrono::steady_clock::time_point lastFrameTimePoint = Chrono::now();

            while (window.ShouldClose() == false)
            {
                window.PollEvents();
                if (_graphics->RenderAsync(_mainCamera))
                {
                    _physics->Integrate(Time::deltaTime);
                    _scripts->Update();
                    _graphics->WaitForRenderCompletion();
                }

                // Update dt
                auto timePoint = Chrono::now();
                fsec fs = timePoint - lastFrameTimePoint;
                Time::deltaTime = fs.count();

                // 60 FPS lock
                if (Time::deltaTime < 1.0f / 60.0f)
                {
                    long sleepDuration = ((1.0f / 60.0f) - Time::deltaTime) * 1000000000;
                    std::this_thread::sleep_for(std::chrono::nanoseconds(sleepDuration));
                    Time::deltaTime = 1.0f / 60.0f;
                }

                Time::time += Time::deltaTime * Time::timeScale;
                lastFrameTimePoint = timePoint;
                
            }

            _graphics->Dispose();
            Editor::DestroyContext();
            _scripts->ShutdownRuntime();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

};

int main()
{
    NoEngine engine;
    engine.Setup();
    return engine.Run();
}