#pragma once

#ifdef NoEngine_Editor
    #include "Core/GameObject.h"
    #include "Core/Scene.h"

    namespace Editor
    {
        void CreateContext();
        void DestroyContext();

        bool Enabled();

        void SetEditorScale(float scale);
        float GetEditorScale();

        void DrawEditors();

        void ShowFPS(float fps);
        void DrawSceneHierarchy(Core::Scene& scene, int* selectionSceneIndex);
        void DrawInspector(Core::GameObject* selectedObject);
    }
#endif

