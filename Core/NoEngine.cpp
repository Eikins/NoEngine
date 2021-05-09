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
#include "Core/Time.h"
#include "Core/Modules/GameManager.hpp"

#include "Graphics/GraphicsContext.h"

#ifdef NoEngine_Editor
#include "Editor/Editor.h"
#include "imgui.h"
#endif

using namespace std;
using namespace Graphics;
using namespace Math;
using namespace Core;

typedef std::chrono::high_resolution_clock Chrono;
typedef std::chrono::nanoseconds ns;
typedef std::chrono::duration<float> fsec;

class NoEngine
{
    const float MAX_DELTA_TIME = 1.0f / 15.0f;

    GameManager _gameManager;
    PhysicsSystem* _physics = nullptr;
    CollisionSystem* _collisionWorld = nullptr;
    GraphicsSystem* _graphics = nullptr;
    ScriptSystem* _scripts = nullptr;

    InputMaster _inputMaster;

    Camera* _mainCamera = nullptr;

    void RegisterComponents()
    {
        _gameManager.RegisterComponent<Camera>();
        _gameManager.RegisterComponent<Renderer>();
        _gameManager.RegisterComponent<AABBCollider>();
        _gameManager.RegisterComponent<Rigidbody>();
        _gameManager.RegisterComponent<Light>();
        _gameManager.RegisterComponent<ScriptedBehaviour>();
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
        _scripts->InitRuntime();
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
        // ========================== LOAD ASSETS ==========================
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
        clownfishMaterial.GetPropertyBlock().SetVector("Color", Math::Vector4(0.2, 0.2, 0.8, 1.0));
        clownfishMaterial.GetPropertyBlock().SetVector("IndirectColor", Math::Vector4(0.05, 0.05, 0.15, 1.0));

        // Scripts
        Script sinWaveScript("SinWave", "Scripts/SinWave.cs");
        Script rotatorScript("Rotator", "Scripts/Rotator.cs");
        Script colorChangerScript("ColorChanger", "Scripts/ColorChanger.cs");
        Script cameraControllerScript("CameraController", "Scripts/CameraController.cs");

        // Scene
        Scene scene("Example Scene");
        // ========================== Create Game Objects ==========================
        GameObject* camera = _gameManager.CreateGameObject("Main Camera");
        GameObject* mainLight = _gameManager.CreateGameObject("Main Light");
        GameObject* empty = _gameManager.CreateGameObject("Empty");
            GameObject* barracuda = _gameManager.CreateGameObject("Barracuda", empty->GetTransform());
        GameObject* clownfish = _gameManager.CreateGameObject("Clownfish");
            GameObject* clownfishChild1 = _gameManager.CreateGameObject("Clownfish Child", clownfish->GetTransform());
            GameObject* clownfishChild2 = _gameManager.CreateGameObject("Clownfish Child", clownfish->GetTransform());
        GameObject* ground = _gameManager.CreateGameObject("Ground");
            GameObject* upperGround = _gameManager.CreateGameObject("Upper Ground", ground->GetTransform());
            GameObject* lowerGround = _gameManager.CreateGameObject("Lower Ground", ground->GetTransform());
                GameObject* abyss = _gameManager.CreateGameObject("Abyss", lowerGround->GetTransform());

        // TODDO : Automatize
        scene.rootTransforms.push_back(camera->GetTransform());
        scene.rootTransforms.push_back(mainLight->GetTransform());
        scene.rootTransforms.push_back(empty->GetTransform());
        scene.rootTransforms.push_back(clownfish->GetTransform());
        scene.rootTransforms.push_back(ground->GetTransform());

        // Main Camera Components
        Camera& mainCamera = _gameManager.AddComponent<Camera>(camera);
        auto& cameraScriptBehaviour = _gameManager.AddComponent<ScriptedBehaviour>(camera);
        cameraScriptBehaviour.AddScript(&cameraControllerScript);
        _inputMaster.RegisterEventReceiver(&cameraScriptBehaviour.GetInstances()[0]);

        // Main Light Components
        auto& light = _gameManager.AddComponent<Light>(mainLight);
        light._type = LightType::DIRECTIONAL;

        // Barracuda
        auto& barracudaRenderer = _gameManager.AddComponent<Renderer>(barracuda);
        auto& barracudaAABB = _gameManager.AddComponent<AABBCollider>(barracuda);
        auto& barracudaRb = _gameManager.AddComponent<Rigidbody>(barracuda);
        barracudaRenderer.mesh = &barracudaMeshes[0];
        barracudaRenderer.material = &barracudaMaterial;
        barracudaAABB.bounds = Bounds(Vector3(0, 0.7f, 0), Vector3(10, 2, 1));
        auto& barracudaScript = _gameManager.AddComponent<ScriptedBehaviour>(barracuda);
        barracudaScript.AddScript(&sinWaveScript);

        // Clownfish & childs
        auto& clownfishRenderer = _gameManager.AddComponent<Renderer>(clownfish);
        auto& clownfishAABB = _gameManager.AddComponent<AABBCollider>(clownfish);
        auto& clownfishBehaviour = _gameManager.AddComponent<ScriptedBehaviour>(clownfish);
        clownfishRenderer.mesh = &clownfishMeshes[0];
        clownfishRenderer.material = &clownfishMaterial;
        clownfishAABB.bounds = Bounds(Vector3::Zero, Vector3::One * 5);
        clownfishBehaviour.AddScript(&colorChangerScript);
        clownfishBehaviour.AddScript(&rotatorScript);

        auto& clownfishChild1Renderer = _gameManager.AddComponent<Renderer>(clownfishChild1);
        auto& clownfishChild2Renderer = _gameManager.AddComponent<Renderer>(clownfishChild2);
        auto& clownfishChild1Behaviour = _gameManager.AddComponent<ScriptedBehaviour>(clownfishChild1);
        auto& clownfishChild2Behaviour = _gameManager.AddComponent<ScriptedBehaviour>(clownfishChild2);
        auto& clownfishChild2AABB = _gameManager.AddComponent<AABBCollider>(clownfishChild2);
        clownfishChild1Renderer.mesh = &clownfishMeshes[0];
        clownfishChild1Renderer.material = &clownfishMaterial;
        clownfishChild2Renderer.mesh = &clownfishMeshes[0];
        clownfishChild2Renderer.material = &clownfishMaterial;
        clownfishChild1Behaviour.AddScript(&rotatorScript);
        clownfishChild2Behaviour.AddScript(&sinWaveScript);
        clownfishChild2AABB.bounds = Bounds(Vector3::Zero, Vector3::One * 5);

        // Ground
        auto& upperGroundAABB = _gameManager.AddComponent<AABBCollider>(upperGround);
        auto& lowerGroundAABB = _gameManager.AddComponent<AABBCollider>(lowerGround);
        upperGroundAABB.bounds = Bounds(Vector3::Zero, Vector3(20, 2, 20));
        lowerGroundAABB.bounds = Bounds(Vector3::Zero, Vector3(40, 2, 20));

        // Set Transforms
        camera->GetTransform()->SetPosition(Vector3(0, 0, -10));

        barracuda->GetTransform()->SetScale(Vector3::One * 0.2f);
        barracuda->GetTransform()->SetPosition(Vector3::Up * 10.0f);

        clownfish->GetTransform()->SetPosition(Vector3(-5, 0, 0));
        clownfish->GetTransform()->SetScale(Vector3::One * 0.5f);
            clownfishChild1->GetTransform()->SetPosition(Vector3(0, 0, 10));
            clownfishChild1->GetTransform()->SetScale(Vector3::One * 0.2f);
            clownfishChild2->GetTransform()->SetPosition(Vector3(-4, 1, -3));
            clownfishChild2->GetTransform()->SetScale(Vector3::One * 0.4f);

        ground->GetTransform()->SetPosition(Vector3(0, -10, 0));
            upperGround->GetTransform()->SetPosition(Vector3(-20, -10, 0));
            lowerGround->GetTransform()->SetPosition(Vector3(0, -20, 0));
            abyss->GetTransform()->SetPosition(Vector3(0, -100, 0));

        _mainCamera = &mainCamera;
        _gameManager.SetScene(&scene);

#ifdef NoEngine_Editor
        Editor::CreateContext(_gameManager);
        Editor::SetEditorScale(2);
#endif

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

            _graphics->CreateContext(windowDescriptor);
            _graphics->BindInputMaster(&_inputMaster);

            Window& window = _graphics->GetWindow();

            Time::deltaTime = 1.0f / 60.0f;
            std::chrono::steady_clock::time_point lastFrameTimePoint = Chrono::now();

            _scripts->ScriptInit();

            while (window.ShouldClose() == false)
            {
                window.PollEvents();
                if (_graphics->RenderAsync(_mainCamera))
                {
                    _physics->Integrate(Time::deltaTime);
                    _scripts->ScriptUpdate();
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
                else if (Time::deltaTime > MAX_DELTA_TIME)
                {
                    Time::deltaTime = MAX_DELTA_TIME;
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