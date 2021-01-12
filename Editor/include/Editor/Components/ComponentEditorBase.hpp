#pragma once

#ifdef NoEngine_Editor
	#include <cctype>

	#include "imgui.h"

	#include "Core/Components/Camera.h"
	#include "Core/Components/Renderer.h"
	#include "Core/Components/ScriptedBehaviour.h"
	#include "Core/Components/AABBCollider.h"
	#include "Core/Components/Rigidbody.h"
	#include "Core/Components/Light.h"
	#include "Core/Assets/Asset.h"

	#include "Core/Modules/GameManager.hpp"

	namespace Editor
	{
		class ComponentEditors
		{
		public:
			static void InputAsset(const char* label, const Core::Asset* asset)
			{
				if (asset == nullptr)
				{
					ImGui::InputText(label, "None", 12, ImGuiInputTextFlags_ReadOnly);
				}
				else
				{
					ImGui::InputText(label, &asset->GetName()[0], 64, ImGuiInputTextFlags_ReadOnly);
				}
			}
		public:
			static void DrawComponents(Core::GameObject* gameObject)
			{
				// Camera
				{
					auto camera = __GameManager->GetComponent<Core::Camera>(gameObject);
					if (camera != nullptr)
					{
						ImGui::PushID(0);
						ImGui::Checkbox("", &static_cast<Core::Component*>(camera)->_enabled);
						ImGui::SameLine();
						DrawCamera(camera);
						ImGui::PopID();
					}
				}

				// Light
				{
					auto light = __GameManager->GetComponent<Core::Light>(gameObject);
					if (light != nullptr)
					{
						ImGui::PushID(100);
						ImGui::Checkbox("", &static_cast<Core::Component*>(light)->_enabled);
						ImGui::SameLine();
						DrawLight(light);
						ImGui::PopID();
					}
				}

				// Renderer
				{
					auto renderer = __GameManager->GetComponent<Core::Renderer>(gameObject);
					if (renderer != nullptr)
					{
						ImGui::PushID(1);
						ImGui::Checkbox("", &static_cast<Core::Component*>(renderer)->_enabled);
						ImGui::SameLine();
						DrawRenderer(renderer);
						ImGui::PopID();
					}
				}

				// AABB Collider
				{
					auto aabb = __GameManager->GetComponent<Core::AABBCollider>(gameObject);
					if (aabb != nullptr)
					{
						ImGui::PushID(2);
						ImGui::Checkbox("", &static_cast<Core::Component*>(aabb)->_enabled);
						ImGui::SameLine();
						DrawAABB(aabb);
						ImGui::PopID();
					}
				}

				// Rigidbody
				{
					auto rb = __GameManager->GetComponent<Core::Rigidbody>(gameObject);
					if (rb != nullptr)
					{
						ImGui::PushID(3);
						ImGui::Checkbox("", &static_cast<Core::Component*>(rb)->_enabled);
						ImGui::SameLine();
						DrawRigidbody(rb);
						ImGui::PopID();
					}
				}

				// ScriptedBehaviour
				{
					auto sb = __GameManager->GetComponent<Core::ScriptedBehaviour>(gameObject);
					if (sb != nullptr)
					{
						ImGui::PushID(4);
						ImGui::Checkbox("", &static_cast<Core::Component*>(sb)->_enabled);
						ImGui::SameLine();
						DrawScriptedBehaviour(sb);
						ImGui::PopID();
					}
				}

			}

			static void DrawTransform(Core::Transform& transform)
			{
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto position = transform._position;
					auto eulerAngles = transform.GetEulerAngles();
					auto scale = transform._scale;

					if (ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position), 0.1f))
					{
						transform.SetPosition(position);
					}

					if (ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&eulerAngles), 0.1f))
					{
						transform.SetEulerAngles(eulerAngles);
					}

					if (ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.1f))
					{
						transform.SetScale(scale);
					}

					if (ImGui::Button("Reset"))
					{
						transform.SetPosition(Math::Vector3::Zero);
						transform.SetRotation(Math::Quaternion::Identity);
						transform.SetScale(Math::Vector3::One);
					}
				}
			}

			static void DrawCamera(Core::Camera* camera)
			{
				if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
				{
					bool hasChanged = false;

					hasChanged |= ImGui::DragFloat("Field of View", &(camera->_fieldOfView), 0.1F, 0.0F, 0.0F, "%.1f");
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

			static void DrawLight(Core::Light* light)
			{
				if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
				{
					const std::array<char*, 1> items = { "Directional" };
					int type = static_cast<int>(light->_type);
					Math::Vector3 color = light->_color;
					
					if (ImGui::Combo("Type", &type, items.data(), items.size()))
					{
						light->_type = static_cast<Core::LightType>(type);
					}

					if (ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&color)))
					{
						light->_color = color;
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

			static void DrawScriptedBehaviour(Core::ScriptedBehaviour* behaviour)
			{
				if (behaviour->script == nullptr)
				{
					if (ImGui::CollapsingHeader("Missing Script (Script)", ImGuiTreeNodeFlags_DefaultOpen))
					{
						InputAsset("Script", nullptr);
					}
				}
				else
				{
					if (ImGui::CollapsingHeader((behaviour->script->GetName() + " (Script)").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						InputAsset("Script", behaviour->script);
						auto& fields = behaviour->GetInstance().GetPublicFields();
						for (auto& field : fields)
						{
							std::string name = field.name;
							name[0] = toupper(name[0]);
							switch (field.type)
							{
							case Scripting::FieldType::FLOAT:
								{
									float value = behaviour->GetInstance().GetFieldValue<float>(field.handle);
									if (ImGui::DragFloat(name.c_str(), &value, 0.1f))
									{
										behaviour->GetInstance().SetFieldValue<float>(field.handle, value);
									}
								}
								break;
							}
						}
					}
				}
			}

			static void DrawAABB(Core::AABBCollider* aabb)
			{
				if (ImGui::CollapsingHeader("AABB Collider", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto& bounds = aabb->bounds;
					auto center = bounds.Center();
					auto size = bounds.Size();

					bool changed = ImGui::DragFloat3("Center", reinterpret_cast<float*>(&center), 0.1f);
					changed |= ImGui::DragFloat3("Size", reinterpret_cast<float*>(&size), 0.1f);
					if (changed)
					{
						bounds = Math::Bounds(center, size);
					}
				}
			}

			static void DrawRigidbody(Core::Rigidbody* rb)
			{
				if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (ImGui::Button("Jump !"))
					{
						rb->velocity = Math::Vector3::Up * 5;
					}
				}
			}
		};
	}
#endif