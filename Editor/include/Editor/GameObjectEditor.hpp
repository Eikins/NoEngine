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
            static void Draw(Core::GameObject* gameObject)
            {
                ImGui::Checkbox("", &gameObject->_active);
                ImGui::SameLine();
                ImGui::InputText("Name", reinterpret_cast<char*>(&gameObject->_name[0]), 64);

                ImGui::Separator();
                ComponentEditors::DrawTransform(gameObject->_transform);
                ImGui::Separator();
                ComponentEditors::DrawComponents(gameObject);
                ImGui::Separator();
                auto renderer = __GameManager->GetComponent<Core::Renderer>(gameObject);
                if (renderer != nullptr)
                {
                    DrawMaterialEditor(renderer->material);
                    if (renderer->material->IsShared())
                    {
                        ImGui::Spacing();
                        if (ImGui::Button("Create Material Instance"))
                        {
                            renderer->material = new Core::Material(*(renderer->material));
                            renderer->hasChanged = true;
                        }
                    }
                }
                ImGui::End();
            }

            static void DrawMaterialEditor(Core::Material* material)
            {
                if (material == nullptr)
                {
                    ImGui::CollapsingHeader("Missing Material", ImGuiTreeNodeFlags_Leaf);
                }
                else
                {
                    if (ImGui::CollapsingHeader((std::string("Material (") + material->GetName() + std::string(")")).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        auto& propertyBlock = material->GetPropertyBlock();
                        auto& props = propertyBlock.Properties();
                        ImGui::Text("Shaders");
                        ComponentEditors::InputAsset("", material->GetVertexShader());
                        ComponentEditors::InputAsset("", material->GetFragmentShader());
                        ImGui::Text("Exposed Properties");
                        for (auto& kp : propertyBlock._nameToProperty)
                        {
                            auto& name = kp.first;
                            auto& prop = props[kp.second];
                            switch (prop.type)
                            {
                            case Core::MaterialPropertyType::COLOR:
                            {
                                auto color = prop.vectorValue;
                                if (ImGui::ColorEdit4(name.c_str(), reinterpret_cast<float*>(&color)))
                                {
                                    prop.vectorValue = color;
                                    propertyBlock._hasChanged = true;
                                }
                            }
                            break;
                            case Core::MaterialPropertyType::VECTOR4:
                            {
                                auto vec4 = prop.vectorValue;
                                if (ImGui::InputFloat4(name.c_str(), reinterpret_cast<float*>(&vec4)))
                                {
                                    prop.vectorValue = vec4;
                                    propertyBlock._hasChanged = true;
                                }
                            }
                            break;
                            }
                        }
                    }
                }
            }
	    };
    }
#endif