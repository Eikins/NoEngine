#pragma once

#include "Math/Math.h"

#include "Component.h"

#ifdef NoEngine_Editor
namespace Editor
{
	class ComponentEditors;
}
#endif

namespace Core
{
	class Camera : public Component
	{
#ifdef NoEngine_Editor
		friend class Editor::ComponentEditors;
#endif
	private:
		float _fieldOfView = 90.0f;
		float _aspectRatio = 1.0f;
		float _nearPlane = 0.3f;
		float _farPlane = 1000.0f;

		bool _hasChanged = true;
		Math::Matrix4x4 _projectionMatrix;
	public:
		Camera() : Component(ComponentType::Camera) {}

		void SetAspectRatio(float aspectRatio);

		Math::Matrix4x4 GetProjectionMatrix();
	};
}