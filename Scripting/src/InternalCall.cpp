#include "Scripting/InternalCall.h"

#include "Core/Modules/GameManager.hpp"

namespace Scripting
{
	void InternalCall::RegisterInternalCalls(Runtime& runtime)
	{
		runtime.RegisterInternalCall("NoEngine.Script::GetComponentHandle", Script_GetComponentHandle);

		// Transform
		runtime.RegisterInternalCall("NoEngine.Transform::SetPosition", Transform_SetPosition);
		runtime.RegisterInternalCall("NoEngine.Transform::SetRotation", Transform_SetRotation);
		runtime.RegisterInternalCall("NoEngine.Transform::SetScale", Transform_SetScale);

		runtime.RegisterInternalCall("NoEngine.Transform::GetPosition", Transform_GetPosition);
		runtime.RegisterInternalCall("NoEngine.Transform::GetRotation", Transform_GetRotation);
		runtime.RegisterInternalCall("NoEngine.Transform::GetScale", Transform_GetScale);

		runtime.RegisterInternalCall("NoEngine.Transform::GetRightVector", Transform_GetRightVector);
		runtime.RegisterInternalCall("NoEngine.Transform::GetUpVector", Transform_GetUpVector);
		runtime.RegisterInternalCall("NoEngine.Transform::GetForwardVector", Transform_GetForwardVector);
		// Renderer
		runtime.RegisterInternalCall("NoEngine.Renderer::GetMaterial", Renderer_GetMaterial);
		runtime.RegisterInternalCall("NoEngine.Material::SetColor", Material_SetColor);
	}

#pragma region Transform
	void InternalCall::Transform_SetPosition(Core::Transform* transform, Math::Vector3 position) { transform->SetPosition(position); }
	void InternalCall::Transform_SetRotation(Core::Transform* transform, Math::Quaternion rotation) { transform->SetRotation(rotation); }
	void InternalCall::Transform_SetScale(Core::Transform* transform, Math::Vector3 scale) { transform->SetScale(scale); }

	Math::Vector3 InternalCall::Transform_GetPosition(Core::Transform* transform) { return transform->GetPosition(); }
	Math::Quaternion InternalCall::Transform_GetRotation(Core::Transform* transform) { return transform->GetRotation(); }
	Math::Vector3 InternalCall::Transform_GetScale(Core::Transform* transform) { return transform->GetScale(); }

	Math::Vector3 InternalCall::Transform_GetForwardVector(Core::Transform* transform) { return transform->ForwardVector(); }
	Math::Vector3 InternalCall::Transform_GetUpVector(Core::Transform* transform) { return transform->UpVector(); }
	Math::Vector3 InternalCall::Transform_GetRightVector(Core::Transform* transform) { return transform->RightVector(); }
#pragma endregion

#pragma region Component
	Core::Component* InternalCall::Script_GetComponentHandle(Core::Transform* transform, int type)
	{
		return static_cast<Core::Component*>(Core::GameManager::Get()->GetComponentRaw(transform->GetGameObject(), type));
	}
#pragma endregion

#pragma region Renderer
	Core::Material* InternalCall::Renderer_GetMaterial(Core::Renderer* renderer) { return renderer->material; }

	void InternalCall::Material_SetColor(Core::Material* material, MonoString* str, Math::Vector4 color)
	{
		auto& name = std::string(mono_string_to_utf8(str));
		material->GetPropertyBlock().SetVector(name, color);
	}
#pragma endregion




}