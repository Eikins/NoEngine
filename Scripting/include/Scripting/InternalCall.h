#pragma once

#include <mono/jit/jit.h>

#include "Scripting/Runtime.h"

#include "Math/Math.h"
#include "Core/Transform.h"
#include "Core/Components/Component.h"
#include "Core/Components/Renderer.h"

namespace Scripting
{
	class InternalCall
	{
	public:
		static void RegisterInternalCalls(Runtime& runtime);
		// Transform
		static void Transform_SetPosition(Core::Transform* transform, Math::Vector3 position);
		static void Transform_SetRotation(Core::Transform* transform, Math::Quaternion rotation);
		static void Transform_SetScale(Core::Transform* transform, Math::Vector3 scale);

		static Math::Vector3 Transform_GetPosition(Core::Transform* transform);
		static Math::Quaternion Transform_GetRotation(Core::Transform* transform);
		static Math::Vector3 Transform_GetScale(Core::Transform* transform);

		static Math::Vector3 Transform_GetForwardVector(Core::Transform* transform);
		static Math::Vector3 Transform_GetUpVector(Core::Transform* transform);
		static Math::Vector3 Transform_GetRightVector(Core::Transform* transform);

		// Component
		static Core::Component* Script_GetComponentHandle(Core::Transform*, int type);

		// Renderer
		static Core::Material* Renderer_GetMaterial(Core::Renderer* renderer);
		static void Material_SetColor(Core::Material* material, MonoString* str, Math::Vector4 color);
	};

}