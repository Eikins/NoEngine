#pragma once

#ifdef NoEngine_Editor
    #include "Core/GameObject.h"
    #include "Core/Assets/Scene.h"
    #include "Core/Components/Camera.h"

    namespace Editor
    {
        static Core::GameManager* __GameManager;

        void CreateContext(Core::GameManager& gameManager);
        void DestroyContext();

        bool Enabled();

        void SetEditorScale(float scale);
        float GetEditorScale();

        void DrawEditors();

        void ShowFPS(float fps);
        void DrawSceneHierarchy(Core::Scene& scene, std::vector<Core::Transform*>& roots, Core::GameObject** selectedObject);
        void DrawInspector(Core::GameObject* selectedObject);

        void SetupCameraProperties(Core::Camera& camera);

        void DrawGizmos(Core::Scene& scene);

        void DrawLine(const Math::Vector3& from, const Math::Vector3& to, const Math::Vector3& color, float thickness = 1.0f);
    }
#endif

