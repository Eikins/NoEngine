#pragma once

#ifdef NoEngine_Editor
    #include "imgui.h"

    #include "Core/GameObject.h"

    #include "Components/ComponentEditorBase.hpp"

    namespace Editor
    {
	    class GameObjectEditor
	    {
        public:
            static void Draw(Core::GameObject& gameObject)
            {
                ImGui::InputText("Name", reinterpret_cast<char*>(&gameObject._name), 16);

                ImGui::Separator();
                ComponentEditors::DrawTransform(gameObject._transform);
                ImGui::Separator();
                ComponentEditors::DrawComponents(gameObject);
                ImGui::End();
            }
	    };
    }
#endif