#pragma once

#include "imgui.h"

#include "Core/Components/Camera.h"

namespace Editor
{
	class CameraEditor
	{
	public:
		static void Draw(Core::Camera& camera)
		{
			Core::Transform* transform = camera.GetTransform();
			bool hasChanged = false;

			ImGui::Text("Transform");
			hasChanged |= ImGui::InputFloat3("Position", reinterpret_cast<float*>(&(transform->_position)));
			Math::Vector3 eulerAngles = transform->_rotation.GetEulerAngles();
			if (ImGui::InputFloat3("Rotation", reinterpret_cast<float*>(&eulerAngles)))
			{
				transform->_rotation = Math::Quaternion::Euler(eulerAngles);
				hasChanged = true;
			}
			hasChanged |= ImGui::InputFloat3("Scale", reinterpret_cast<float*>(&(transform->_scale)));
			if (hasChanged)
			{
				transform->_hasChanged = true;
			}

			bool hasChanged = false;
			ImGui::Text("Camera");
			hasChanged |= ImGui::InputFloat("Field of View", &camera._fieldOfView, 0.0F, 0.0F, "%.1f");
			hasChanged |= ImGui::InputFloat("Aspect Ratio", &camera._aspectRatio);
			hasChanged |= ImGui::InputFloat("Near Plane", &camera._nearPlane, 0.0F, 0.0F, "%.2f");
			hasChanged |= ImGui::InputFloat("Far Plane", &camera._farPlane, 0.0F, 0.0F, "%.1f");
			
			if (hasChanged)
			{
				camera._hasChanged = true;
			}

			if (ImGui::Button("Print Camera Properties"))
			{
				std::cout << "View Matrix:" << std::endl;
				std::cout << camera.GetTransform()->GetLocalToWorldMatrix().InverseTR();
				std::cout << "Projection Matrix:" << std::endl;
				std::cout << camera.GetProjectionMatrix();
			}
		}
	};
}