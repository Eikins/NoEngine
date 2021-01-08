#ifdef NoEngine_Editor
    #include "Editor/Editor.h"
    #include "Editor/GameObjectEditor.hpp"
    #include "Editor/SceneEditors.h"

    #include "Core/Components/Camera.h"

    #include "imgui.h"

    namespace Editor
    {
        bool __editorEnabled = false;
        float __editorScale = 1.0f;

        Math::Matrix4x4 __gizmoProjViewMatrix;
        Math::Matrix4x4 __gizmoMatrix;

        bool __drawGizmosBounds = false;

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
            ImGuiStyle& style = ImGui::GetStyle();
            style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
            style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
            style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
            style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
            style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
            style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
            style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
            style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
            style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
            style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
            style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
            style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
            //style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
            //style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
            style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
            style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
            style.GrabRounding = style.FrameRounding = 2.3f;

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

        void DrawSceneHierarchy(Core::Scene& scene, std::vector<Core::Transform*>& roots, Core::GameObject** selectedObject)
        {
            auto io = ImGui::GetIO();
            ImGui::SetNextWindowSize(ImVec2(400, io.DisplaySize.y), ImGuiCond_Once);
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

            ImGui::Begin("Hierarchy");
            ImGui::SetWindowFontScale(__editorScale);
                SceneEditors::DrawSceneHierarchy(roots, selectedObject);
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
                GameObjectEditor::Draw(*selectedObject);
            }
            ImGui::End();
        }

        void SetupCameraProperties(Core::Camera& camera)
        {
            __gizmoProjViewMatrix = camera.GetProjectionMatrix() * camera.GetTransform()->GetLocalToWorldMatrix().InverseTR();
            __gizmoMatrix = Math::Matrix4x4::Identity;
        }

        inline Math::Vector2 TransformToScreenPos(const ImGuiIO& io, const Math::Vector3& position)
        {
            auto csPos = __gizmoProjViewMatrix * __gizmoMatrix * Math::Vector4(position, 1.0f);
            csPos = csPos / csPos.w;
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

        void DrawGizmos(Core::Scene& scene)
        {
            auto io = ImGui::GetIO();
            int posX = io.DisplaySize.x - 600;
            ImGui::SetNextWindowPos(ImVec2(posX, 0), ImGuiCond_Once);

            ImGui::Begin("Gizmos");
            ImGui::SetWindowFontScale(__editorScale);

            ImGui::Checkbox("Draw Bounds", &__drawGizmosBounds);
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