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
                for (uint32_t i = 0; i < gameObject._components.size(); i++)
                {
                    ImGui::PushID(i);
                    ComponentEditors::DrawComponent(gameObject._components[i]);
                    ImGui::PopID();
                }
                ImGui::End();
            }
	    };
    }
#endif