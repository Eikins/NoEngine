#ifdef NoEngine_Editor
    #include "Editor/Editor.h"
    #include "Editor/GameObjectEditor.hpp"
    #include "Editor/SceneEditors.h"

    #include "Core/Components/Camera.h"
    #include "Core/Systems/ScriptSystem.h"

    #include "imgui.h"

    namespace Editor
    {
        bool __editorEnabled = false;
        float __editorScale = 1.0f;

        Math::Matrix4x4 __gizmoProjViewMatrix;
        Math::Matrix4x4 __gizmoMatrix;

        bool __drawGizmosBounds = false;
        bool __drawLightDir = false;

        Core::GameObject* __selectedObject = nullptr;

        void SetEditorScale(float scale)
        {
            __editorScale = scale;
        }

        float GetEditorScale()
        {
            return __editorScale;
        }

        void CreateContext(Core::GameManager& gameManager)
        {
            __GameManager = &gameManager;
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            (void)io;

            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Dear ImGui style
            //ImGui::StyleColorsDark();
            //ImGui::StyleColorsClassic();
        
            // From community imgui styles
            ImGuiStyle* style = &ImGui::GetStyle();
            ImVec4* colors = style->Colors;

            colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
            colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
            colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
            colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
            colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
            colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
            colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
            colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
            colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
            colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
            colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
            colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
            colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
            colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
            colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
            colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
            colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
            colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
            colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
            colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
            colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
            colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
            colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
            colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
            colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
            colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
            colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
            colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
            colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
            colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
            colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
            colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
            //colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
            //colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
            colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
            colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
            colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
            colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
            colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
            colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
            colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

            style->ChildRounding = 4.0f;
            style->FrameBorderSize = 1.0f;
            style->FrameRounding = 2.0f;
            style->GrabMinSize = 7.0f;
            style->PopupRounding = 2.0f;
            style->ScrollbarRounding = 12.0f;
            style->ScrollbarSize = 13.0f;
            style->TabBorderSize = 1.0f;
            style->TabRounding = 0.0f;
            style->WindowRounding = 4.0f;

            __editorEnabled = true;
        }

        void DestroyContext()
        {
            ImGui::DestroyContext();
            __editorEnabled = false;
        }

        bool Enabled()
        {
            return __editorEnabled;
        }

        void DrawEditors()
        {
            Editor::DrawSceneHierarchy(__GameManager->GetScene(), &__selectedObject);
            Editor::DrawInspector(__selectedObject);
        }

        void ShowFPS(float fps)
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::Begin("Framerate", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs);
            ImGui::SetWindowFontScale(__editorScale);
            ImColor color;
            if (fps <= 15)
            {
                color = ImColor(208, 0, 0);
            }
            else if (fps < 30)
            { 
                color = ImColor(255, 186, 8);
            }
            else
            {
                color = ImColor(255, 255, 255);
            }
            ImGui::TextColored(color, "FPS: %d", static_cast<int>(fps));
            ImGui::End();
        }

        void DrawSceneHierarchy(Core::Scene* scene, Core::GameObject** selectedObject)
        {
            auto io = ImGui::GetIO();
            ImGui::SetNextWindowSize(ImVec2(400, io.DisplaySize.y), ImGuiCond_Once);
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

            ImGui::Begin("Hierarchy");
            ImGui::SetWindowFontScale(__editorScale);
                SceneEditors::DrawSceneHierarchy(scene->rootTransforms, selectedObject);
            ImGui::End();
        }

        void DrawInspector(Core::GameObject* selectedObject)
        {
            auto io = ImGui::GetIO();
            int posX = io.DisplaySize.x - 400;
            ImGui::SetNextWindowSize(ImVec2(400, io.DisplaySize.y), ImGuiCond_Once);
            ImGui::SetNextWindowPos(ImVec2(posX, 0), ImGuiCond_Once);

            ImGui::Begin("Inspector");
            ImGui::SetWindowFontScale(__editorScale);
            if (selectedObject != nullptr)
            {
                GameObjectEditor::Draw(selectedObject);
            }
            ImGui::End();
        }

        void SetupCameraProperties(Core::Camera* camera)
        {
            __gizmoProjViewMatrix = camera->GetProjectionMatrix() * camera->GetTransform()->GetLocalToWorldMatrix().InverseTR();
            __gizmoMatrix = Math::Matrix4x4::Identity;
        }

        inline Math::Vector2 TransformToScreenPos(const ImGuiIO& io, const Math::Vector3& position)
        {
            auto csPos = __gizmoProjViewMatrix * __gizmoMatrix * Math::Vector4(position, 1.0f);
            csPos = csPos / Math::Abs(csPos.w);
            return (Math::Vector2(csPos.x, csPos.y) + Math::Vector2::One) / 2 * Math::Vector2(io.DisplaySize.x, io.DisplaySize.y);
        }

        void DrawLine(const Math::Vector3& from, const Math::Vector3& to, const Math::Vector3& color, float thickness)
        {
            auto& io = ImGui::GetIO();

            auto drawList = ImGui::GetBackgroundDrawList();
            auto ssFrom = TransformToScreenPos(io, from);
            auto ssTo = TransformToScreenPos(io, to);

            drawList->AddLine(ImVec2(ssFrom.x, ssFrom.y), ImVec2(ssTo.x, ssTo.y), ImColor(color.x * 255, color.y * 255, color.z * 255), thickness);
        }

        void DrawGizmos()
        {
            auto io = ImGui::GetIO();
            int posX = io.DisplaySize.x - 600;
            ImGui::SetNextWindowPos(ImVec2(posX, 0), ImGuiCond_Once);

            ImGui::Begin("Editor Settings");
            ImGui::SetWindowFontScale(__editorScale);

            ImGui::Checkbox("Show Bounds", &__drawGizmosBounds);
            ImGui::Checkbox("Show Light Direction", &__drawLightDir);

            //if (ImGui::Button("Reload Scripts"))
            //{
            //    auto scriptSystem = __GameManager->GetSystem<Core::ScriptSystem>();
            //    scriptSystem->Reload();
            //    scriptSystem->CompileAndLoadCore();
            //    scriptSystem->CompileAndLoadScripts();
            //}

            if (__drawLightDir)
            {
                auto lights = __GameManager->GetAllComponents<Core::Light>();
                if (lights.size() > 0)
                {
                    auto directionalLight = lights[0];
                    auto worldPos = directionalLight.GetTransform()->GetWorldPosition();
                    auto dirPos = worldPos + directionalLight.GetTransform()->ForwardVector();
                    DrawLine(worldPos, dirPos, directionalLight._color);
                }
            }

            if (__drawGizmosBounds)
            {
                auto aabbs = __GameManager->GetAllComponents<Core::AABBCollider>();
                for (auto& aabb : aabbs)
                {
                    //__gizmoMatrix = aabb.GetTransform()->GetLocalToWorldMatrix();
                    auto bounds = aabb.GetWorldBounds();
                    Math::Vector3 color(0, 0.8, 0);

                    DrawLine(bounds.min, Math::Vector3(bounds.max.x, bounds.min.y, bounds.min.z), color);
                    DrawLine(bounds.min, Math::Vector3(bounds.min.x, bounds.max.y, bounds.min.z), color);
                    DrawLine(bounds.min, Math::Vector3(bounds.min.x, bounds.min.y, bounds.max.z), color);

                    DrawLine(bounds.max, Math::Vector3(bounds.min.x, bounds.max.y, bounds.max.z), color);
                    DrawLine(bounds.max, Math::Vector3(bounds.max.x, bounds.min.y, bounds.max.z), color);
                    DrawLine(bounds.max, Math::Vector3(bounds.max.x, bounds.max.y, bounds.min.z), color);

                    DrawLine(Math::Vector3(bounds.min.x, bounds.max.y, bounds.min.z), Math::Vector3(bounds.max.x, bounds.max.y, bounds.min.z), color);
                    DrawLine(Math::Vector3(bounds.min.x, bounds.max.y, bounds.min.z), Math::Vector3(bounds.min.x, bounds.max.y, bounds.max.z), color);

                    DrawLine(Math::Vector3(bounds.max.x, bounds.min.y, bounds.max.z), Math::Vector3(bounds.max.x, bounds.min.y, bounds.min.z), color);
                    DrawLine(Math::Vector3(bounds.max.x, bounds.min.y, bounds.max.z), Math::Vector3(bounds.min.x, bounds.min.y, bounds.max.z), color);

                    DrawLine(Math::Vector3(bounds.min.x, bounds.min.y, bounds.max.z), Math::Vector3(bounds.min.x, bounds.max.y, bounds.max.z), color);
                    DrawLine(Math::Vector3(bounds.max.x, bounds.min.y, bounds.min.z), Math::Vector3(bounds.max.x, bounds.max.y, bounds.min.z), color);
                }
            }
        }
    }
#endif