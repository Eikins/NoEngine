#pragma once

#ifdef NoEngine_Editor
	#include "imgui.h"

	#include "Core/Components/Camera.h"
	#include "Core/Components/Renderer.h"
	#include "Core/Assets/Asset.h"

	namespace Editor
	{
		class ComponentEditors
		{
		private:
			static void InputAsset(const char* label, const Core::Asset* asset)
			{
				if (asset == nullptr)
				{
					ImGui::InputText(label, "None", 12, ImGuiInputTextFlags_ReadOnly);
				}
				else
				{
					ImGui::InputText(label, &asset->GetName()[0], 16, ImGuiInputTextFlags_ReadOnly);
				}
			}
		public:
			static void DrawTransform(Core::Transform& transform)
			{
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto position = transform._position;
					auto eulerAngles = transform._rotation.GetEulerAngles();
					auto scale = transform._scale;

					if (ImGui::InputFloat3("Position", reinterpret_cast<float*>(&position)))
					{
						transform.SetPosition(position);
					}

					if (ImGui::InputFloat3("Rotation", reinterpret_cast<float*>(&eulerAngles)))
					{
						transform.SetRotation(Math::Quaternion::Euler(eulerAngles));
					}

					if (ImGui::InputFloat3("Scale", reinterpret_cast<float*>(&scale)))
					{
						transform.SetScale(scale);
					}
				}
			}

			static void DrawComponent(Core::Component* component)
			{
				switch (component->GetType())
				{
				case Core::ComponentType::Camera:
					DrawCamera(static_cast<Core::Camera*>(component));
					break;
				case Core::ComponentType::Renderer:
					DrawRenderer(static_cast<Core::Renderer*>(component));
					break;
				default: break;
				}
			}

			static void DrawCamera(Core::Camera* camera)
			{
				if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
				{
					bool hasChanged = false;

					hasChanged |= ImGui::InputFloat("Field of View", &(camera->_fieldOfView), 0.0F, 0.0F, "%.1f");
					hasChanged |= ImGui::InputFloat("Aspect Ratio", &(camera->_aspectRatio));
					hasChanged |= ImGui::InputFloat("Near Plane", &(camera->_nearPlane), 0.0F, 0.0F, "%.2f");
					hasChanged |= ImGui::InputFloat("Far Plane", &(camera->_farPlane), 0.0F, 0.0F, "%.1f");

					if (hasChanged)
					{
						camera->_hasChanged = true;
					}

					if (ImGui::Button("Print Camera Properties"))
					{
						std::cout << "View Matrix:" << std::endl;
						std::cout << camera->GetTransform()->GetLocalToWorldMatrix().InverseTR();
						std::cout << "Projection Matrix:" << std::endl;
						std::cout << camera->GetProjectionMatrix();
					}
				}
			}

			static void DrawRenderer(Core::Renderer* renderer)
			{
				if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
				{
					InputAsset("Mesh", renderer->mesh);
					InputAsset("Material", renderer->material);
				}
			}
		};
	}
#endif